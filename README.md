The goal is to implement a fast, embedded system on an atmega328p using first principles (without looking anything up, other than the DFT math) in order to make a visualizer for music, and then benchmark that against a more informed approach in a separate branch.

# Blocks

## Clean Up Analog Signal
- Set up simple analog RC LPF for >48 kHz.
- Process both left and right signal

## Process Signal

- Configure atmega328p ADC's bit resolution and sample rate to be compatible with the 48 kHz, which should be flexible. This is slow enough that I expect a high resolution. From the data sheet, a single conversion (other than the first) requires 13 cycles, which gives considerable oversampling overhead for the full depth of 10 bits.
- Use Signal Processing Toolbox to create a digital LPF.

## Write FFT

- Set up fixed-point arithmetic and test boundary cases <- IN PROGRESS
    - Since the ADC resolution is 10-bit, the whole part of the fixed-point representation must be able to express that directly without overflow. All complexmultiplications are of sub-1 magnitude, sotwo 10-bit signed integers are sufficient for a single complex term, but the summation of these complex terms may result in multiple bit shifts. For now I'll be working under a 1024-sample FFT, which could result in an approximate shift of 10 bits. As a result, my fixed-point representation is 1 sign bit-21 whole bits-10 fractional bits

- Consider sample buffer size/DFT point counts.
- Try rectangular representation of complex values using two arrays for complex transformation.
- Test against sine wave audio, determine methods to benchmark computation speed.

## Hook to visualizer

- Many options from LCD to ferrofluid, consider more when basics are done.
