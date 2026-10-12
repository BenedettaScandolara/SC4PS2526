# Homework 05 – Stability of Legendre Polynomials

Assignment: [HomeWork05](https://codimd.infn.it/s/2dwLWV8e3).
I compute the Legendre polynomials $P_\ell(x)$ for $m = 0$, $\ell = 0,\dots,50$ and $x = 0.1,\ 0.5,\ 0.9,\ 0.99$ with three methods, and I compare them.

## How to run

You need `gcc` and Python with `mpmath` and `matplotlib` (`pip install mpmath matplotlib`).

```
make
```

All CSV files and plots are saved in the `output` folder.

## What the code does

- `legendre.c` computes $P_\ell(x)$ in double precision with:
  - the **forward recurrence** (Bonnet), starting from $P_0 = 1$, $P_1 = x$;
  - the **backward recurrence** (Miller-style), starting from $\widetilde P_{51} = 0$, $\widetilde P_{50} = 1$, going down to $\ell = 0$ and then rescaling with $c = 1/\widetilde P_0$.

  The results are saved in `output/legendre_x*.csv`.
- `reference_and_plots.py`:
  - computes the **high-precision reference** with `mpmath`, using the same forward recurrence with 50 decimal digits (and checking it against the `mpmath.legendre` function);
  - computes the absolute and relative errors of the two double-precision methods (`output/errors_x*.csv`);
  - makes the plots.

## Results

- `output/P_values.png`: $P_\ell(x)$ versus $\ell$ for the three methods
- `output/relative_error.png`: relative error versus $\ell$
- `output/absolute_error.png`: absolute error versus $\ell$

Maximum relative error for $\ell = 0,\dots,50$:

| x | forward | backward |
|---|---|---|
| 0.1 | 1.3e-14 | 1.7e+02 |
| 0.5 | 1.2e-15 | 4.8e+01 |
| 0.9 | 9.3e-15 | 8.1e+00 |
| 0.99 | 8.3e-14 | 8.4e+00 |

The results agree with the expected behaviour described in the assignment. The values calculated using the forward recurrence closely follow the high-precision reference values for all the tested values of x. On the other hand, the backward recurrence does not generally reproduce the reference Legendre polynomials.

This result is also confirmed by the relative and absolute errors. The forward recurrence shows very small errors, close to the machine epsilon ($\approx 2.2 \cdot 10^{-16}$), while the errors obtained with the backward recurrence are of order 1 or larger, so the backward values are simply wrong.
For $x = 0.5$ some forward points are missing in the error plots: there the error is exactly zero, because 0.5 is exact in binary.

Therefore, for the tested values of x and for $\ell \le 50$, the forward recurrence appears numerically stable, whereas the arbitrary backward recurrence does not reliably recover $P_\ell(x)$.

## Why

- A three-term recurrence has **two independent solutions**. For Legendre, they are $P_\ell(x)$ and $Q_\ell(x)$ (the Legendre functions of the second kind). Any pair of starting values gives a combination $A\,P_\ell + B\,Q_\ell$.
- **Forward:** starting from $P_0 = 1$, $P_1 = x$ selects exactly $P_\ell$. Rounding errors add a tiny piece of $Q_\ell$, but for $|x| < 1$ both $P_\ell$ and $Q_\ell$ oscillate with similar size, so this piece does not grow. The error stays at the level of the rounding errors.
- **Backward:** the start $\widetilde P_{51} = 0$, $\widetilde P_{50} = 1$ is arbitrary, so the result is a mix of $P_\ell$ and $Q_\ell$. Miller's algorithm works only when the wanted solution is **minimal** in the backward direction: going backward it grows faster than the other solution, and the unwanted part dies out. Here neither solution dominates, so the $Q_\ell$ part never disappears. Rescaling with $c$ only fixes $\ell = 0$ and cannot remove $Q_\ell$.

## Connection to spherical harmonics

$$Y_{\ell 0}(\theta,\phi) = \sqrt{\frac{2\ell+1}{4\pi}}\,P_\ell(\cos\theta)$$

With $x = \cos\theta$, the tested values correspond to $\theta \approx 84.3^\circ,\ 60^\circ,\ 25.8^\circ,\ 8.1^\circ$.
Since $Y_{\ell 0}$ is just $P_\ell$ multiplied by a known factor:
- the **relative error** of $Y_{\ell 0}$ is the same as the one of $P_\ell$;
- the **absolute error** is multiplied by $\sqrt{(2\ell+1)/4\pi}$, which is at most about 2.8 for $\ell = 50$.

So with the forward recurrence also $Y_{\ell 0}$ is accurate to about $10^{-14}$, while with the backward recurrence it would be wrong.

## Conclusions

- The Miller-style backward procedure does not compute $P_\ell(x)$ for $|x| < 1$.
- The ordinary forward recurrence is stable for the tested values.
- Miller's algorithm is useful only when the wanted solution is really minimal.
