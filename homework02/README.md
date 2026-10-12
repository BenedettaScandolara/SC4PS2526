# Homework 02

The program computes the vector sum

```
d = a*x + y
```

where `a` is a scalar and `x`, `y`, `d` are vectors of size N. All the elements of `x` are equal, and all the elements of `y` are equal.

## How to run

```
gcc hw2.c -o hw2 -lm
./hw2 N a x y expected_value
```

Example: `./hw2 10 3 0.1 7.1 7.4`

## What the code does

- It reads N, a, x, y and the expected value from the command line.
- It allocates the vectors with `malloc`, so it also works for very large N.
- It computes `d[i] = a*x[i] + y[i]` for every element.
- It checks every element of `d` against the expected value in two ways: with `==` and with a tolerance (`|d[i] - expected| < 1e-9`). Then it prints how many elements are wrong.

The results for N = 10, 10^6, 10^8 are in the `output` folder.

## Questions

**Did you find problems for some N?**
Yes. In my first version I used fixed arrays (`double x[10]`), and the program crashed (segmentation fault) for N = 10^6. With `malloc` it works for all N.

**Were you able to test a*x + y = 7.4?**
Not with `==`: the test fails for every element. The numbers 0.1 and 7.1 cannot be stored exactly in binary, so the computer gets 7.39999999999999946709 instead of 7.4. With a small tolerance the test passes.
With numbers that are exact in binary, like `a=3, x=0.5, y=7.5`, the result is exactly 9 and `==` works too.
