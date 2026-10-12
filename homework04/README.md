# Homework 04 – FFT

Answers to the worksheet [LESSONS.md](https://github.com/alexninogh/myfft_tutorial/blob/main/LESSONS.md).
The code in `src/`, `include/`, `scripts/` and the `Makefile` comes from the course repository [myfft_tutorial](https://github.com/alexninogh/myfft_tutorial).

## How to run

GNU GSL and Python with matplotlib are needed.

```
make plot-python    # runs both programs and makes the plots
make part7          # runs the modified code of Part 7
```

All CSV files and plots are saved in the `output` folder.

Changes to the original code:
- the plots are saved in `output` (instead of `plots`)
- `part7/sampling_demo.c` is the file modified in Part 7, and `part7/plot_part7.py` makes the before/after plot

## Part 1

1. An FFT is a mathematical operation that analyzes a signal in the frequency domain, showing the characteristic frequencies present in the signal and their amplitudes.
2.
   - The sampling rate represents the number of samples of a signal acquired every second.
   - The Nyquist frequency is the maximum frequency that can be correctly represented for a given sampling rate, and it is equal to half of the sampling rate.
   - The frequency resolution is the distance between two consecutive frequencies analyzed by the FFT, and it is calculated as the sampling rate divided by the number of samples N.
3. A uniformly spaced time grid is important for FFT analysis because it provides a constant sampling rate, which is necessary for the FFT to work correctly.

## Part 2

1. The program produces eight CSV files:
   1. `good_sampling_signal.csv` → the signal in the time domain in the well-sampled case. Plot: `output/sampling_signals.png` (top).
   2. `good_sampling_spectrum.csv` → the FFT spectrum of the well-sampled signal. Plot: `output/sampling_spectra.png` (top).
   3. `undersampled_signal.csv` → the same signal in the time domain, but with a lower sampling rate. Plot: `output/sampling_signals.png` (middle).
   4. `undersampled_spectrum.csv` → the FFT spectrum of the undersampled signal. Plot: `output/sampling_spectra.png` (middle).
   5. `short_record_signal.csv` → a signal observed for a short time interval of 0.125 s. Plot: `output/sampling_signals.png` (bottom).
   6. `short_record_spectrum.csv` → the FFT spectrum of the short-record signal. Plot: `output/sampling_spectra.png` (bottom).
   7. `coupled_oscillators_time.csv` → two coupled oscillators, x₁ and x₂, as functions of time. Plot: `output/coupled_oscillators_time.png`.
   8. `coupled_oscillators_spectrum.csv` → the FFT spectrum of the coupled oscillators. Plot: `output/coupled_oscillators_spectrum.png`.
2. The program that studies sampling issues is `sampling_demo.c`.
3. The program that studies a mechanics problem is `coupled_oscillators_fft.c`.
4. The two frequencies reported in the well-sampled case are 50 Hz and 120 Hz.

## Part 3

2. The Nyquist frequency is $f_N = f_s/2 = 128/2 = 64$ Hz.
3. The 120 Hz component cannot be reconstructed faithfully because it is above the Nyquist frequency of 64 Hz.
5. Physically, if the sampling rate is too low, the samples are too far apart to follow the fast oscillation correctly. The sampled points can then look like a slower oscillation. Mathematically, frequencies separated by multiples of the sampling frequency produce the same sampled sequence. In this case, 120 Hz sampled at 128 Hz appears as 128 − 120 = 8 Hz (see `output/sampling_spectra.png`, middle).

## Part 4

2. The total record length is $T = N\Delta t = 64 \cdot \frac{1}{512}$ s $= 0.125$ s.
3. The frequency spacing of the FFT bins is $\Delta f = 1/T = 8$ Hz.
4. The two frequencies are difficult to separate because they are only 5 Hz apart, while the frequency resolution is 8 Hz. Therefore, the FFT resolution is not sufficient to clearly distinguish the two frequencies.
5. I would increase the total acquisition time. Since $\Delta f = 1/T$, increasing the acquisition time decreases the frequency spacing and improves the frequency resolution. This makes it easier to separate the 50 Hz and 55 Hz components.

## Part 5

1. The equations are:

$$m\ddot{x}_1 = -k x_1 - k_c (x_1 - x_2)$$

$$m\ddot{x}_2 = -k x_2 - k_c (x_2 - x_1)$$

2. The equations describe two masses connected to springs and coupled by another spring.
3. The system has two normal modes: an in-phase mode, where the masses move in the same direction, and an out-of-phase mode, where they move in opposite directions.
4. The FFT of x₁ shows two important frequencies because the motion contains both normal modes.
5. Comparing the FFT peaks with the theoretical frequencies is useful to verify that the numerical simulation agrees with the physical theory.

## Part 6

1. The undersampled spectrum makes aliasing easiest to see, because the original 120 Hz component appears as a false peak at 8 Hz.
2. The short-record spectrum makes the limited frequency resolution easiest to see, because the 50 Hz and 55 Hz components cannot be clearly separated.
3. Yes:

   |          | In-phase mode | Out-of-phase mode |
   |----------|---------------|-------------------|
   | expected | 0.79577 Hz    | 0.99392 Hz        |
   | measured | 0.79688 Hz    | 0.99219 Hz        |

   The numerical FFT peaks line up very well with the theoretical reference frequencies (see `output/coupled_oscillators_spectrum.png`).
4. The small differences are mainly due to the finite frequency resolution of the FFT. Here $\Delta f = 1/128$ s $\approx 0.0078$ Hz, and the differences (0.0011 Hz and 0.0017 Hz) are smaller than one bin.

## Part 7 – Option A

I changed the sampling rate of the undersampled case from 128 Hz to 256 Hz (`part7/sampling_demo.c`).

**Prediction:** with $f_s = 256$ Hz the Nyquist frequency is 128 Hz, which is above both 50 Hz and 120 Hz. So there should be no aliasing, and the peaks should appear at 50 Hz and 120 Hz.

**Result:** the plot before and after the change is `output/part7_spectrum_before_after.png`.
The FFT output agrees with my prediction. After increasing the sampling rate from 128 Hz to 256 Hz, the peaks appear at 50 Hz and 120 Hz. The alias peak at 8 Hz disappears because the Nyquist frequency is now 128 Hz, which is higher than both signal frequencies.

## Part 8

Thanks to this project, I have learnt that analysing a signal is not a trivial task, but requires a thorough understanding of how the modelling system works in order to understand if the analysis is numerically stable. In particular, I have learnt how the sampling rate and acquisition time can influence the results of an FFT, especially in terms of aliasing and frequency resolution.
About software organization, I learnt that it is useful to keep source files, headers and outputs in separate folders, to put the shared code (`fft_utils.c`) in one place used by both programs, and to use a Makefile so that compiling, running and plotting are done with one command.
I would like to extend this project by adding random noise to the signal, to see if the FFT can still find the two peaks.
