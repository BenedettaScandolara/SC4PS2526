#!/usr/bin/env python3
# Part 7: undersampled spectrum before (fs = 128 Hz) and after (fs = 256 Hz)

import csv
from pathlib import Path

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

ROOT = Path(__file__).resolve().parent.parent
OUTPUT_DIR = ROOT / "output"


def read_csv(path):
    f, a = [], []
    with path.open() as handle:
        reader = csv.reader(handle)
        next(reader)
        for row in reader:
            f.append(float(row[0]))
            a.append(float(row[1]))
    return f, a


before = read_csv(OUTPUT_DIR / "undersampled_spectrum.csv")
after = read_csv(OUTPUT_DIR / "part7" / "undersampled_spectrum.csv")

fig, axes = plt.subplots(2, 1, figsize=(10, 6), constrained_layout=True)
fig.suptitle("Part 7: undersampled case, before and after changing the sampling rate")

axes[0].vlines(before[0], 0.0, before[1], linewidth=1.4)
axes[0].set_title("Before: fs = 128 Hz (Nyquist = 64 Hz)")
axes[1].vlines(after[0], 0.0, after[1], linewidth=1.4)
axes[1].set_title("After: fs = 256 Hz (Nyquist = 128 Hz)")

for ax in axes:
    ax.set_xlim(0.0, 130.0)
    ax.set_xlabel("Frequency (Hz)")
    ax.set_ylabel("Amplitude")
    ax.grid(True, alpha=0.3)

fig.savefig(OUTPUT_DIR / "part7_spectrum_before_after.png", dpi=150)
print("wrote output/part7_spectrum_before_after.png")
