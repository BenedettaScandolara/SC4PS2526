# Homework 03 – Matrix Multiplication

The program computes the matrix product

```
C = A B      C_ij = sum_k A_ik B_kj
```

with `A = a 1_N` and `B = b 1_N`, where `1_N` is the N x N identity matrix. So the expected result is `C = ab 1_N`: `a*b` on the diagonal and `0` everywhere else.

## How to run

```
gcc -O2 hw3.c -o hw3
./hw3 a b N fileout
```

Example: `./hw3 2 3 5 C.txt`

## What the code does

- It reads a, b, N and the output file name from the command line.
- It allocates A, B, C with `malloc`. Each matrix is stored as one array of N*N elements, row by row, so `M[i][j]` is `M[i*N + j]`.
- It computes C with all 6 orders of the three loops (ijk, ikj, jik, jki, kij, kji) and measures the time of each one with `clock()`.
- After each multiplication it checks C (see below).
- It saves C to `fileout`, one row per line.

The `output` folder contains the matrix for N = 5 (`C_N5.txt`) and the benchmark for N = 256, 512, 1024 (`benchmark.txt`).

## Questions

**Check that every element of C = ab. Is there a quicker way?**
The full check compares all N² elements: `a*b` on the diagonal, `0` outside.
A quicker way is to compare only two numbers: the trace of C (sum of the diagonal) and the sum of all elements. Both must be `N*a*b`. This is 2 comparisons instead of N². It is not a perfect proof (two errors could cancel each other), but it catches almost every mistake.

**Which loop ordering is the fastest and why?**
Times for N = 1024 (compiled with `-O2`):

| order | time (s) |
|---|---|
| ikj | 0.73 |
| kij | 0.69 |
| ijk | 7.53 |
| jik | 7.79 |
| jki | 11.48 |
| kji | 11.19 |

The fastest are **ikj** and **kij**, about 10 times faster than ijk and 15 times faster than jki and kji.

The reason is how C stores matrices in memory: row by row. The CPU loads memory into the cache in small blocks of neighbouring elements, so reading elements one after the other is fast, while jumping around is slow.

- **ikj, kij**: the inner loop is on `j`, so `B[k][j]` and `C[i][j]` are read one after the other along a row. Fastest.
- **ijk, jik**: the inner loop is on `k`, so `B[k][j]` goes down a column and jumps N elements every step. Slow.
- **jki, kji**: the inner loop is on `i`, so both `A[i][k]` and `C[i][j]` go down a column. Slowest.
