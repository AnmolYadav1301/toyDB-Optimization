import pandas as pd
import matplotlib.pyplot as plt

# Load CSV
df = pd.read_csv("pf_results.csv")

# Split into LRU and MRU
lru = df[df['policy'] == 1]
mru = df[df['policy'] == 2]

# Metrics to plot: (column_name, pretty_label)
metrics = [
    ("logicalReads", "Logical Reads"),
    ("logicalWrites", "Logical Writes"),
    ("physicalReads", "Physical Reads"),
    ("physicalWrites", "Physical Writes"),
    ("cacheHits", "Cache Hits"),
    ("cacheMisses", "Cache Misses"),
]

# Create subplots
fig, axes = plt.subplots(3, 2, figsize=(14, 12))
axes = axes.flatten()

for ax, (metric, label) in zip(axes, metrics):
    ax.plot(lru['writePct'], lru[metric], marker='o', label='LRU')
    ax.plot(mru['writePct'], mru[metric], marker='o', label='MRU')

    ax.set_title(label, fontsize=12)
    ax.set_xlabel("Write Percentage (%)")
    ax.set_ylabel(label)
    ax.grid(True)
    ax.legend()

plt.tight_layout()
plt.savefig("combined_pf_plot.png", dpi=300)
plt.show()

print("Saved combined plot as combined_pf_plot.png")
