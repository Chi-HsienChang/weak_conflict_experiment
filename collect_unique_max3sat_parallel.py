#!/usr/bin/env python3
import subprocess as sp
import re
import argparse
from pathlib import Path
from datetime import datetime
from concurrent.futures import ThreadPoolExecutor, as_completed
from threading import Lock
from shutil import which
import sys

# 偵測輸出文字
UNIQUE_NEG_PATTERN = re.compile(r"global optimal is not unique", re.IGNORECASE)
SEED_PATTERN = re.compile(r"Seed\s*=\s*(\d+)")

def run_once(cmd):
    """執行一次可執行檔，回傳 (stdout, seed:int|None, is_unique:bool)"""
    proc = sp.run(cmd, text=True, capture_output=True)
    out = (proc.stdout or "") + (("\n[STDERR]\n" + proc.stderr) if proc.stderr else "")
    m = SEED_PATTERN.search(out)
    seed = int(m.group(1)) if m else None
    is_unique = not UNIQUE_NEG_PATTERN.search(out)  # 沒看到「not unique」就視為唯一
    return out, seed, is_unique

def resolve_bin(bin_arg: str) -> str:
    """回傳可執行檔的『絕對路徑』，避免 'main' 被當成 PATH 查找失敗。"""
    bin_path = Path(bin_arg)
    # 1) 直接指定的檔案
    if bin_path.is_file():
        return str(bin_path.resolve())
    # 2) 常見候選
    for p in (Path("./main"), Path("./build/main")):
        if p.is_file():
            return str(p.resolve())
    # 3) PATH 尋找
    found = which(str(bin_arg))
    if found:
        return str(Path(found).resolve())
    raise FileNotFoundError(
        f"Cannot find executable. Tried: {bin_path}, ./main, ./build/main, and PATH lookup for {bin_arg}"
    )

def main():
    parser = argparse.ArgumentParser(
        description="Collect unique-optimal Max3SAT instances by seed (parallel)."
    )
    parser.add_argument("--bin", default="./main", help="path to executable (default: ./main)")
    parser.add_argument("--args", nargs="*", default=["8", "20", "1", "0", "0", "1"],
                        help="arguments to pass to the binary")
    parser.add_argument("--target", type=int, default=5, help="how many unique instances to collect")
    parser.add_argument("--max-runs", type=int, default=10000, help="max attempts total")
    parser.add_argument("--outdir", default="unique_instances", help="directory to store txt outputs")
    parser.add_argument("--jobs", type=int, default=4, help="parallel workers (suggest: CPU cores)")
    args = parser.parse_args()

    # 解析並顯示實際使用的執行檔路徑
    real_bin = resolve_bin(args.bin)
    base_cmd = [real_bin] + args.args
    print(f"[info] using executable: {real_bin}")
    print(f"[info] base command: {' '.join(base_cmd)}")

    outdir = Path(args.outdir)
    outdir.mkdir(parents=True, exist_ok=True)

    log_path = outdir / "collect_log.csv"
    if not log_path.exists():
        log_path.write_text("timestamp,seed,is_unique,filename,cmd\n", encoding="utf-8")

    collected = 0
    attempts = 0
    seen_seeds = set()
    lock = Lock()

    def one_attempt():
        nonlocal attempts
        out, seed, is_unique = run_once(base_cmd)
        seed_str = str(seed) if seed is not None else f"noseed_{datetime.now().strftime('%Y%m%d_%H%M%S_%f')}"
        ts = datetime.now().isoformat()

        with lock:
            attempts += 1
            # 跳過重複 seed（平行情境下要鎖）
            if seed in seen_seeds:
                log_path.open("a", encoding="utf-8").write(
                    f"{ts},{seed_str},{is_unique},,{' '.join(base_cmd)}\n"
                )
                return None
            if seed is not None:
                seen_seeds.add(seed)

            if is_unique:
                filename = outdir / f"max3sat_unique_seed_{seed_str}.txt"
                filename.write_text(out, encoding="utf-8")
                log_path.open("a", encoding="utf-8").write(
                    f"{ts},{seed_str},True,{filename.name},{' '.join(base_cmd)}\n"
                )
                print(f"[SAVE] unique -> {filename}")
                return ("saved", seed_str)
            else:
                log_path.open("a", encoding="utf-8").write(
                    f"{ts},{seed_str},False,,{' '.join(base_cmd)}\n"
                )
                return None

    try:
        # 平行提交批次工作，直到蒐集滿 target 或達到 max-runs
        with ThreadPoolExecutor(max_workers=max(1, args.jobs)) as ex:
            futures = set()
            # 先填滿佇列
            for _ in range(min(args.max_runs, args.jobs)):
                futures.add(ex.submit(one_attempt))

            while futures and collected < args.target and attempts < args.max_runs:
                done = {f for f in futures if f.done()}
                futures -= done
                for f in done:
                    res = f.result()
                    if res and res[0] == "saved":
                        collected += 1
                        print(f"[{collected}/{args.target}] collected (seed={res[1]})")
                    # 動態補工作
                    if attempts < args.max_runs and collected < args.target:
                        futures.add(ex.submit(one_attempt))

            # 若還沒達標，等現有任務跑完（不再補新任務）
            for f in as_completed(futures):
                res = f.result()
                if collected >= args.target:
                    break
                if res and res[0] == "saved":
                    collected += 1
                    print(f"[{collected}/{args.target}] collected (seed={res[1]})")
                    if collected >= args.target:
                        break

        if collected >= args.target:
            print(f"Done. Collected {collected} unique-optimal instances into: {outdir}")
        else:
            print(f"Stopped after {attempts} runs, collected {collected}/{args.target}. See log: {log_path}")

    except KeyboardInterrupt:
        print("\n[Interrupted] Gracefully stopping...", file=sys.stderr)
        print(f"Progress: {collected}/{args.target}, attempts={attempts}", file=sys.stderr)

if __name__ == "__main__":
    main()
