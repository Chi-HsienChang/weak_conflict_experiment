# -*- coding: utf-8 -*-
# For every file matching 'max3sat_unique_seed_*.txt', extract the 20x20 "EG"
# adjacency matrix block and draw a directed graph where 1=solid(red), 2=dashed(blue).
#
# Outputs per file (with seed tag):
#   - eg_graph_circular_seed_<SEED>.png
#   - eg_graph_kamada_seed_<SEED>.png
#
# Requirements: numpy, matplotlib, networkx

import re
from pathlib import Path
import numpy as np
import matplotlib.pyplot as plt
import networkx as nx

# ---- drawing function ----
def draw_from_matrix(mat: np.ndarray, layout: str, out_path: Path):
    n = mat.shape[0]
    G = nx.DiGraph()
    G.add_nodes_from(range(n))

    # encode edges: 1 -> solid (red), 2 -> dashed (blue)
    for i in range(n):
        for j in range(n):
            val = mat[i, j]
            if val in (1, 2):
                G.add_edge(i, j, style=("solid" if val == 1 else "dashed"))

    if layout == "circular":
        pos = nx.circular_layout(G)
    elif layout == "kamada":
        pos = nx.kamada_kawai_layout(G, weight=None)
    else:
        pos = nx.spring_layout(G, seed=42)

    plt.figure(figsize=(9, 9))

    # nodes: white fill, black edge
    nx.draw_networkx_nodes(G, pos, node_size=520, node_color="white",
                           edgecolors="black", linewidths=1.5)

    solid = [(u, v) for u, v, d in G.edges(data=True) if d.get("style") == "solid"]
    dashed = [(u, v) for u, v, d in G.edges(data=True) if d.get("style") == "dashed"]

    # solid edges in red
    nx.draw_networkx_edges(
        G, pos, edgelist=solid, arrows=True, edge_color="red",
        arrowstyle='-|>', connectionstyle='arc3,rad=0.09'
    )
    # dashed edges in blue
    nx.draw_networkx_edges(
        G, pos, edgelist=dashed, arrows=True, edge_color="blue",
        style='dashed', arrowstyle='-|>', connectionstyle='arc3,rad=0.09'
    )

    nx.draw_networkx_labels(G, pos, font_size=10, font_color="black")

    plt.axis("off")
    plt.title(f"Directed Graph from EG Matrix ({layout})")
    plt.tight_layout()
    plt.savefig(out_path, dpi=300)
    plt.close()
    print(f"[info] Saved: {out_path}")


# ---- main loop for all files ----
for src in Path(".").glob("max3sat_unique_seed_*.txt"):
    print(f"[info] Processing: {src.name}")
    text = src.read_text(encoding="utf-8")

    # ---- extract 20x20 EG matrix block ----
    lines = text.splitlines()
    start_idx = None
    for i, ln in enumerate(lines):
        if ln.strip() == "EG":
            start_idx = i
            break
    if start_idx is None:
        print(f"[warn] Could not find 'EG' in {src.name}, skipped.")
        continue

    matrix_lines = []
    for ln in lines[start_idx + 1:]:
        ln_stripped = ln.strip()
        if not ln_stripped:
            if matrix_lines:
                break
            else:
                continue
        if re.fullmatch(r"[0-2](?:\s+[0-2]){19}", ln_stripped):
            matrix_lines.append(ln_stripped)
            if len(matrix_lines) == 20:
                break

    if len(matrix_lines) != 20:
        print(f"[warn] Did not capture 20x20 matrix in {src.name}, skipped.")
        continue

    mat = np.array([list(map(int, row.split())) for row in matrix_lines], dtype=int)

    # ---- extract seed from filename ----
    m = re.search(r"seed_(\d+)", src.name)
    seed_str = m.group(1) if m else "unknown"

    # ---- generate outputs ----
    out1 = Path(f"eg_graph_circular_seed_{seed_str}.png")
    out2 = Path(f"eg_graph_kamada_seed_{seed_str}.png")
    draw_from_matrix(mat, "circular", out1)
    draw_from_matrix(mat, "kamada", out2)
