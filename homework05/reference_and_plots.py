#!/usr/bin/env python3
# High-precision reference with mpmath, errors and plots

import csv
from pathlib import Path

import mpmath
from mpmath import mp
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

mp.dps = 50  # 50 decimal digits (double has about 16)

OUTPUT = Path(__file__).resolve().parent / "output"
X_VALUES = ["0.1", "0.5", "0.9", "0.99"]
L = 50
EPS = 2.220446049250313e-16  # machine epsilon in double precision


def reference(x_string):
    # same forward recurrence, but with 50 digits
    x = mp.mpf(float(x_string))  # same double value of x used by the C code
    P = [mp.mpf(1), x]
    for l in range(1, L):
        P.append((2*l + 1) * x * P[l] / (l + 1) - l * P[l - 1] / (l + 1))
    # check against the mpmath built-in function
    for l in range(L + 1):
        assert abs(P[l] - mpmath.legendre(l, x)) < mp.mpf(10) ** -40
    return P


def read_double(x_string):
    l, fw, bw = [], [], []
    with open(OUTPUT / f"legendre_x{x_string}.csv") as f:
        reader = csv.reader(f)
        next(reader)
        for row in reader:
            l.append(int(row[0]))
            fw.append(float(row[1]))
            bw.append(float(row[2]))
    return l, fw, bw


def nan_if_zero(v):
    # errors below 1e-30 are zero within the reference precision,
    # and zero cannot be drawn on a log scale
    return v if v > 1e-30 else float("nan")


results = {}
for xs in X_VALUES:
    l, fw, bw = read_double(xs)
    ref = reference(xs)
    abs_fw = [abs(mp.mpf(fw[i]) - ref[i]) for i in l]
    abs_bw = [abs(mp.mpf(bw[i]) - ref[i]) for i in l]
    rel_fw = [abs_fw[i] / abs(ref[i]) for i in l]
    rel_bw = [abs_bw[i] / abs(ref[i]) for i in l]
    results[xs] = dict(l=l, fw=fw, bw=bw, ref=[float(r) for r in ref],
                       abs_fw=[float(e) for e in abs_fw], abs_bw=[float(e) for e in abs_bw],
                       rel_fw=[float(e) for e in rel_fw], rel_bw=[float(e) for e in rel_bw])

    with open(OUTPUT / f"errors_x{xs}.csv", "w", newline="") as f:
        w = csv.writer(f)
        w.writerow(["l", "reference", "forward", "backward",
                    "abs_err_forward", "abs_err_backward", "rel_err_forward", "rel_err_backward"])
        r = results[xs]
        for i in l:
            w.writerow([i, f"{r['ref'][i]:.17e}", f"{fw[i]:.17e}", f"{bw[i]:.17e}",
                        f"{r['abs_fw'][i]:.3e}", f"{r['abs_bw'][i]:.3e}",
                        f"{r['rel_fw'][i]:.3e}", f"{r['rel_bw'][i]:.3e}"])

    r = results[xs]
    print(f"x = {xs}: max rel. error forward = {max(r['rel_fw']):.1e}, "
          f"backward = {max(r['rel_bw']):.1e}")


def figure(title):
    fig, axes = plt.subplots(2, 2, figsize=(12, 8), constrained_layout=True)
    fig.suptitle(title)
    return fig, axes.flatten()


# 1) values of P_l(x)
fig, axes = figure("Legendre polynomials P_l(x): forward, backward and reference")
for ax, xs in zip(axes, X_VALUES):
    r = results[xs]
    ax.plot(r["l"], r["ref"], "k-", linewidth=3, alpha=0.3, label="reference (mpmath)")
    ax.plot(r["l"], r["fw"], "o-", markersize=3, label="forward")
    ax.plot(r["l"], r["bw"], "s-", markersize=3, label="backward")
    ax.set_title(f"x = {xs}")
    ax.set_xlabel("l")
    ax.set_ylabel("P_l(x)")
    ax.grid(True, alpha=0.3)
    ax.legend()
fig.savefig(OUTPUT / "P_values.png", dpi=150)
plt.close(fig)

# 2) relative and absolute errors
for kind, label in [("rel", "Relative error"), ("abs", "Absolute error")]:
    fig, axes = figure(f"{label} with respect to the high-precision reference")
    for ax, xs in zip(axes, X_VALUES):
        r = results[xs]
        ax.semilogy(r["l"], [nan_if_zero(e) for e in r[f"{kind}_fw"]], "o-", markersize=3, label="forward")
        ax.semilogy(r["l"], [nan_if_zero(e) for e in r[f"{kind}_bw"]], "s-", markersize=3, label="backward")
        ax.axhline(EPS, color="gray", linestyle="--", linewidth=1, label="machine epsilon")
        ax.set_title(f"x = {xs}")
        ax.set_xlabel("l")
        ax.set_ylabel(label.lower())
        ax.grid(True, alpha=0.3)
        ax.legend()
    name = "relative_error.png" if kind == "rel" else "absolute_error.png"
    fig.savefig(OUTPUT / name, dpi=150)
    plt.close(fig)

print("plots saved in output/")
