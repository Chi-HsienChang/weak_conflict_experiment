#!/usr/bin/env python3
"""
check_max3sat_unique.py

輸入: 子句檔案（每行一 clause, 例如 "1 -3 4" 表示 (x1 OR ¬x3 OR x4)）。
用途: 找到能滿足最多 clause 的 assignment，並檢查是否唯一。
"""

import sys
import itertools

def parse_clause_file(path):
    clauses = []
    max_var = 0
    with open(path, 'r', encoding='utf-8') as f:
        for raw in f:
            line = raw.strip()
            if not line or line.startswith('#'):
                continue
            # 支援以 0 結尾或不以 0 結尾
            parts = line.split()
            lits = []
            for p in parts:
                try:
                    v = int(p)
                except:
                    continue
                if v == 0:
                    break
                lits.append(v)
                max_var = max(max_var, abs(v))
            if lits:
                clauses.append(tuple(lits))
    return clauses, max_var

def eval_clause(clause, assignment):  # assignment: list/tuple of booleans indexed 1..n (0 unused)
    # clause is tuple of ints like (1, -2, 3)
    for lit in clause:
        var = abs(lit)
        val = assignment[var]
        if lit > 0 and val:
            return True
        if lit < 0 and not val:
            return True
    return False

def assignment_from_int(mask, n):
    # return 1-indexed list: index 0 unused
    a = [False] * (n + 1)
    for i in range(1, n+1):
        a[i] = bool((mask >> (i-1)) & 1)
    return a

def stringify_assignment(assignment):
    # assignment is 1-indexed list of bools
    return ' '.join([f"x{i}={'1' if assignment[i] else '0'}" for i in range(1, len(assignment))])

def main():
    if len(sys.argv) < 2:
        print("Usage: python check_max3sat_unique.py <clause_file>")
        sys.exit(1)
    path = sys.argv[1]
    clauses, n = parse_clause_file(path)
    if n == 0:
        print("No variables detected.")
        sys.exit(1)
    m = len(clauses)
    print(f"Loaded {m} clauses, variables n={n}")

    best = -1
    best_masks = []

    # 如果變數很多，提醒並退出（安全檢查）
    if n > 26:
        print("警告: 變數數量 n > 26，暴力列舉可能非常慢。若確實要繼續，請修改程式或使用 MaxSAT 求解器。")
        # 這裡我們仍繼續，但你可以把上面條件改成更嚴格或直接 sys.exit(1).

    total = 1 << n
    # 可以做簡單剪枝：先計算每子句的變數集合，略過（此版直接暴力）
    for mask in range(total):
        assignment = assignment_from_int(mask, n)
        sat = 0
        # 小優化：逐子句檢查並累加
        for clause in clauses:
            if eval_clause(clause, assignment):
                sat += 1
        if sat > best:
            best = sat
            best_masks = [mask]
        elif sat == best:
            best_masks.append(mask)

    print(f"Max satisfied clauses = {best} out of {m}")
    if len(best_masks) == 0:
        print("No satisfying assignment found (異常).")
        return

    if len(best_masks) == 1:
        print("唯一最優解 (unique optimal assignment).")
    else:
        print(f"最優解個數 = {len(best_masks)} (not unique).")

    # 顯示最優解（若太多，只顯示前 20 個）
    display_limit = 20
    for i, mask in enumerate(best_masks[:display_limit]):
        assign = assignment_from_int(mask, n)
        print(f"[{i+1}] mask={mask} -> {stringify_assignment(assign)}")

    if len(best_masks) > display_limit:
        print(f"... 還有 {len(best_masks)-display_limit} 個最優解未列出。")

if __name__ == "__main__":
    main()
