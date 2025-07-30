The goal is to implement a fast, embedded system on an atmega328p using first principles (without looking anything up, other than the DFT math) in order to make a visualizer for music, and then benchmark that against a more informed approach in a separate branch.

# Blocks

## Clean Up Analog Signal

- Set up simple analog RC low-pass filter for 44.1 kHz.

## Process Signal

- Configure atmega328p ADC's bit resolution and sample rate. There is a tradeoff here, so when the system is built, that needs to be investigated.

## Write FFT

- Set up fixed-point arithmetic and test boundary cases <- IN PROGRESS
- Try rectangular representation of complex values using two arrays.
- Consider sample buffer size/DFT point counts.
- Test against sine wave audio, determine methods to benchmark computation speed.

## Hook to visualizer

- Many options from LCD to ferrofluid, consider more when basics are done.
