# Homework 06 – Split the work

The program computes the vector sum `d = a*x + y` (as in Homework 02), but it splits the work in chunks.

## How to run

You need the HDF5 library (on Mac: `brew install hdf5`).

```
make run
```

This compiles `hw6.c` and runs `./hw6 input.txt`.
To inspect the HDF5 file: `h5dump output/chunks.h5`

## Input file

All the inputs are read from `input.txt`, written as `Variable = Value`:

```
N = 100
chunk_size = 8
a = 3
x = 0.1
y = 7.1
output_file = output/chunks.h5
```

To read it I used the suggested library [Config-Parser-C](https://github.com/welljsjs/Config-Parser-C) (the file `config-parser/parser.h`). It returns a list of (key, value) pairs, and I compare each key with `strcmp`.

## What the code does

1. It computes `d` with a single `for`, as in the original code, and the sum of all its elements.
2. It computes `d` again in chunks:
   - number of chunks = N / chunk_size rounded up, computed as `(N + chunk_size - 1) / chunk_size`. With N = 100 and chunk_size = 8 there are 13 chunks, and the last one has only 4 elements.
   - for chunk `c`: `start = c*chunk_size`, `end = start + chunk_size` (or `N` for the last chunk). In C the arrays start from 0, so I don't use the `+1` of the pseudocode, and `end` is excluded.
   - for every chunk it also computes the sum of its elements and puts it in `partial_chunk_sum`.
3. It sums all the elements of `partial_chunk_sum`.
4. It saves every chunk (`/chunks/chunk_000` … `/chunks/chunk_012`) and `partial_chunk_sum` in an HDF5 file.

## Results

The output of the program is in `output/run_log.txt`, the HDF5 file is `output/chunks.h5`, and its content in text form is in `output/chunks_h5dump.txt`.

**Check 1: is `d` the same as the original code?**
Yes, 0 elements are different out of 100. Every element is computed with the same operation `a*x[i] + y[i]`, so the result is exactly the same.

**Check 2: is the sum of the partial sums the same as the sum of `d`?**
Not exactly:

```
original sum = 739.99999999999864
chunk sum    = 740.00000000000011
```

The relative difference is about 2e-15, so they are equal within rounding errors, but not with `==`.
The reason is that the additions are done in a different order. With floating point numbers each addition is rounded, so the order changes the rounding: (a + b) + c is not always exactly equal to a + (b + c).
The chunk sum is actually closer to the exact value (100 × d[0] ≈ 740), because it adds small partial sums instead of adding each element to one big number.

With bigger vectors the effect is clearer. With N = 1000000 and chunk_size = 1000:

```
original sum = 7400000.0001241192
chunk sum    = 7399999.9999999497
```
