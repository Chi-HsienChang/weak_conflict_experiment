# Read the uploaded file, extract the 20x20 "EG" adjacency matrix block,
# and draw a directed graph where 1=solid edge, 2=dashed edge.
#
# The script will create two outputs:
#   - eg_graph_circular.png : circular layout
#   - eg_graph_kamada.png   : kamada-kawai layout (often clearer)
#
# You can reuse the `draw_from_matrix` function with any 0/1/2 matrix.

import re
from pathlib import Path
import numpy as np
import matplotlib.pyplot as plt
import networkx as nx

src = Path("./max3sat_unique_seed_657013513.txt")
text = src.read_text(encoding="utf-8")

# Extract the block after "EG" that consists of 20 lines of 20 integers
lines = text.splitlines()
start_idx = None
for i, ln in enumerate(lines):
    if ln.strip() == "EG":
        start_idx = i
        break

if start_idx is None:
    raise RuntimeError("Could not find 'EG' section in the file.")

# Find the first block of lines that look like '0 1 2 ...' after "EG"
matrix_lines = []
for ln in lines[start_idx+1:]:
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
    # ignore headings like "====" or others

if len(matrix_lines) != 20:
    raise RuntimeError("Did not capture a 20x20 adjacency matrix after 'EG'.")

mat = np.array([list(map(int, row.split())) for row in matrix_lines], dtype=int)

def draw_from_matrix(mat: np.ndarray, layout: str, out_path: Path):
    n = mat.shape[0]
    G = nx.DiGraph()
    G.add_nodes_from(range(n))

    # 1 -> solid edge, 2 -> dashed edge
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
    nx.draw_networkx_nodes(G, pos, node_size=520)

    solid = [(u, v) for u, v, d in G.edges(data=True) if d.get("style") == "solid"]
    dashed = [(u, v) for u, v, d in G.edges(data=True) if d.get("style") == "dashed"]

    nx.draw_networkx_edges(G, pos, edgelist=solid, arrows=True, arrowstyle='-|>', connectionstyle='arc3,rad=0.09')
    nx.draw_networkx_edges(G, pos, edgelist=dashed, arrows=True, style='dashed', arrowstyle='-|>', connectionstyle='arc3,rad=0.09')
    nx.draw_networkx_labels(G, pos, font_size=10)

    plt.axis("off")
    plt.title(f"Directed Graph from EG Matrix ({layout})")
    plt.tight_layout()
    plt.savefig(out_path, dpi=300)
    plt.close()

out1 = Path("eg_graph_circular.png")
out2 = Path("eg_graph_kamada.png")
draw_from_matrix(mat, "circular", out1)
draw_from_matrix(mat, "kamada", out2)
