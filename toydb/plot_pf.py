import pandas as pd
import matplotlib.pyplot as plt

# Load CSV
df = pd.read_csv("pf_results.csv")

policy_names = {1: "LRU", 2: "MRU"}

metrics = [
    ("logicalReads", "Logical Reads"),
    ("logicalWrites", "Logical Writes"),
    ("physicalReads", "Physical Reads"),
    ("physicalWrites", "Physical Writes"),
    ("cacheHits", "Cache Hits"),
    ("cacheMisses", "Cache Misses"),
]

# Create 2×3 subplot figure
fig, axes = plt.subplots(2, 3, figsize=(16, 10))
axes = axes.flatten()

for ax, (column, pretty) in zip(axes, metrics):
    for policy in [1, 2]:
        df_p = df[df["policy"] == policy]
        ax.plot(
            df_p["writePct"],
            df_p[column],
            marker="o",
            label=policy_names[policy]
        )

    ax.set_title(pretty)
    ax.set_xlabel("Write Percentage (%)")
    ax.set_ylabel(pretty)
    ax.grid(True)
    ax.legend()

plt.tight_layout()
plt.savefig("pf_plot_combined.png", dpi=200)
plt.close()

print("Generated combined plot: pf_plot_combined.png")
