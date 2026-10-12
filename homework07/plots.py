#!/usr/bin/env python3
# Plots for Homework 07 (the data are produced by hw7.c)

import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

OUT = "output/"
LAMBDA = 1.5

# 1) coin tosses
n, frac = np.loadtxt(OUT + "coin.csv", delimiter=",", skiprows=1, unpack=True)
plt.figure(figsize=(8, 4.5))
plt.semilogx(n, frac, label="running fraction of heads")
plt.axhline(0.5, color="k", linestyle="--", label="0.5")
plt.plot(n, 0.5 + 0.5 / np.sqrt(n), "r:", label=r"$0.5 \pm 0.5/\sqrt{n}$")
plt.plot(n, 0.5 - 0.5 / np.sqrt(n), "r:")
plt.ylim(0.3, 0.7)
plt.xlabel("number of tosses n")
plt.ylabel("fraction of heads")
plt.title("Coin tosses")
plt.legend()
plt.grid(alpha=0.3)
plt.savefig(OUT + "coin.png", dpi=150, bbox_inches="tight")
plt.close()

# 2) Monte Carlo pi
N, pi_est, err = np.loadtxt(OUT + "pi.csv", delimiter=",", skiprows=1, unpack=True)
plt.figure(figsize=(8, 4.5))
plt.loglog(N, err, "o-", label="|estimate - pi|")
plt.loglog(N, np.sqrt(np.pi * (4 - np.pi) / N), "k--", label=r"expected error $\approx 1.64/\sqrt{N}$")
plt.xlabel("N")
plt.ylabel("absolute error")
plt.title(r"Monte Carlo estimate of $\pi$")
plt.legend()
plt.grid(alpha=0.3, which="both")
plt.savefig(OUT + "pi_error.png", dpi=150, bbox_inches="tight")
plt.close()

y_u2, y_exp = np.loadtxt(OUT + "samples.csv", delimiter=",", skiprows=1, unpack=True)

# 3) Y = U^2
plt.figure(figsize=(8, 4.5))
plt.hist(y_u2, bins=50, range=(0, 1), density=True, alpha=0.6, label="histogram of Y = U^2")
y = np.linspace(0.005, 1, 400)
plt.plot(y, 1 / (2 * np.sqrt(y)), "r", label=r"$f_Y(y) = 1/(2\sqrt{y})$")
plt.ylim(0, 6)
plt.xlabel("y")
plt.ylabel("density")
plt.title(r"Change of variables: $Y = U^2$")
plt.legend()
plt.grid(alpha=0.3)
plt.savefig(OUT + "u_squared.png", dpi=150, bbox_inches="tight")
plt.close()

# 4) exponential
plt.figure(figsize=(8, 4.5))
plt.hist(y_exp, bins=60, range=(0, 6), density=True, alpha=0.6, label="histogram")
y = np.linspace(0, 6, 400)
plt.plot(y, LAMBDA * np.exp(-LAMBDA * y), "r", label=r"$f(y) = \lambda e^{-\lambda y}$")
plt.xlabel("y")
plt.ylabel("density")
plt.title(r"Inverse transform: exponential with $\lambda = 1.5$")
plt.legend()
plt.grid(alpha=0.3)
plt.savefig(OUT + "exponential.png", dpi=150, bbox_inches="tight")
plt.close()

# 5) empirical CDF
y_sorted = np.sort(y_exp)
ecdf = np.arange(1, len(y_sorted) + 1) / len(y_sorted)
cdf = 1 - np.exp(-LAMBDA * y_sorted)
max_diff = np.max(np.abs(ecdf - cdf))
plt.figure(figsize=(8, 4.5))
plt.step(y_sorted, ecdf, where="post", label="empirical CDF")
plt.plot(y_sorted, cdf, "r--", label=r"exact CDF $1 - e^{-\lambda y}$")
plt.xlim(0, 6)
plt.xlabel("y")
plt.ylabel("F(y)")
plt.title(f"Empirical CDF (max difference = {max_diff:.4f})")
plt.legend()
plt.grid(alpha=0.3)
plt.savefig(OUT + "ecdf.png", dpi=150, bbox_inches="tight")
plt.close()

print(f"mean of the exponential sample = {y_exp.mean():.4f} (expected 1/lambda = {1/LAMBDA:.4f})")
print(f"max |empirical CDF - exact CDF| = {max_diff:.4f}")
print("plots saved in output/")
