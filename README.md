The goal is to implement a fast, embedded music visualizer on an atmega328p using first principles (without looking anything up, other than the DFT math), and then benchmark that against a more informed approach in a separate branch.

# Blocks

## Clean Up Analog Signal
- Set up simple analog RC LPF for >48 kHz.

## Process Signal

- Configure atmega328p ADC's bit resolution and sample rate to be compatible with the 48 kHz, which should be flexible. This is slow enough that I expect a high resolution, even when processing both the left and right signal. From the data sheet, a single conversion (other than the first) requires 13 cycles, which gives considerable oversampling overhead at a 16 MHz CPU frequency for the full depth of 10 bits.
- Use Signal Processing Toolbox to create a digital LPF for both left and right. Then shift each register by 1 to the left and sum to create an averaged mono channel.

## Write FFT

- Set up fixed-point arithmetic and test boundary cases <- IN PROGRESS
    - Since the ADC resolution is 10-bit, the whole part of the fixed-point representation must be able to express that directly without overflow. All complex multiplications are of sub-1 magnitude, so two 10-bit signed integers are sufficient for a single complex term, but the summation of these complex terms may result in multiple bit shifts. For now I'll be working under a 1024-sample DFT, which could result in an approximate shift of 10 bits. As a result, my fixed-point representation is 1 sign bit-21 whole bits-10 fractional bits.

- Try rectangular representation of complex values using two arrays for complex transformation.
- Consider sample buffer size/DFT point counts.
    - Just do Cooley-Tukey
    - Currently using a 1024-sample DFT. In my experience, dynamic programming evaluations require less space than recursive operations, even though recursive operations may be nicer--both can be tested and evaluated.
- Test against sine wave audio, determine methods to benchmark computation speed.

- Metrics:
-   Set up and experiment with picobench

## Hook to visualizer

- Many options from LCD to electromagnet-controlled ferrofluid(!), consider more when basics are done.
