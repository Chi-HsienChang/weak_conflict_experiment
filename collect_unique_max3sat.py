#!/usr/bin/env python3
import subprocess as sp
import re
import argparse
from pathlib import Path
from datetime import datetime

# 用來辨識輸出
UNIQUE_NEG_PATTERN = re.compile(r"global optimal is not unique", re.IGNORECASE)
SEED_PATTERN = re.compile(r"Seed\s*=\s*(\d+)")

def run_once(cmd):
    """Run ./main ... once and return (stdout, seed:int|None, is_unique:bool)."""
    proc = sp.run(cmd, text=True, capture_output=True)
    out = (proc.stdout or "") + (("\n[STDERR]\n" + proc.stderr) if proc.stderr else "")
    # 抓 seed
    m = SEED_PATTERN.search(out)
    seed = int(m.group(1)) if m else None
    # 判斷唯一：現在程式只有在「不唯一」時會輸出提示
    is_unique = not UNIQUE_NEG_PATTERN.search(out)
    return out, seed, is_unique

def main():
    parser = argparse.ArgumentParser(description="Collect unique-optimal Max3SAT instances by seed.")
    parser.add_argument("--bin", default="./main", help="path to executable (default: ./main)")
    parser.add_argument("--args", nargs="*", default=["8", "20", "1", "0", "0", "1"],
                        help="arguments to pass to the binary")
    parser.add_argument("--target", type=int, default=30, help="how many unique instances to collect")
    parser.add_argument("--max-runs", type=int, default=10000, help="max attempts to try before giving up")
    parser.add_argument("--outdir", default="unique_instances", help="directory to store txt outputs")
    args = parser.parse_args()

    cmd = [args.bin] + args.args
    outdir = Path(args.outdir)
    outdir.mkdir(parents=True, exist_ok=True)

    collected = 0
    seen_seeds = set()
    log_path = outdir / "collect_log.csv"
    if not log_path.exists():
        log_path.write_text("timestamp,seed,is_unique,filename,cmd\n", encoding="utf-8")

    for attempt in range(1, args.max_runs + 1):
        out, seed, is_unique = run_once(cmd)

        # 若沒有 seed，就給一個備援名稱
        seed_str = str(seed) if seed is not None else f"noseed_{datetime.now().strftime('%Y%m%d_%H%M%S_%f')}"

        # 跳過重複的 seed
        if seed in seen_seeds:
            log_path.open("a", encoding="utf-8").write(
                f"{datetime.now().isoformat()},{seed_str},{is_unique},,{' '.join(cmd)}\n"
            )
            continue
        if seed is not None:
            seen_seeds.add(seed)

        if is_unique:
            filename = outdir / f"max3sat_unique_seed_{seed_str}.txt"
            filename.write_text(out, encoding="utf-8")
            collected += 1
            print(f"[{collected}/{args.target}] saved unique instance -> {filename}")
            log_path.open("a", encoding="utf-8").write(
                f"{datetime.now().isoformat()},{seed_str},True,{filename.name},{' '.join(cmd)}\n"
            )
            if collected >= args.target:
                print("Done. Collected target number of unique-optimal instances.")
                break
        else:
            log_path.open("a", encoding="utf-8").write(
                f"{datetime.now().isoformat()},{seed_str},False,,{' '.join(cmd)}\n"
            )

    if collected < args.target:
        print(f"Stopped after {args.max_runs} runs, collected {collected}/{args.target} unique cases.")
        print(f"See log: {log_path}")

if __name__ == "__main__":
    main()
