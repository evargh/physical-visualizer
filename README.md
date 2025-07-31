The goal is to implement a fast, embedded music visualizer on an ESP32 using first principles (without looking anything up, other than the DFT math), and then benchmark that against a more informed approach in a separate branch.

Due to a few basics being missed with the ATMEGA328P (namely the internal conversion time taking 13 ADC cycles, not 13 CPU cycles, and the overhead of trying to stream data at any higher rate into a software buffer using SPI to emulate I2S on a device that's also doing an FFT), I'm moving to an ESP32.

# Blocks

## Handle Analog Signal
- Set up simple analog RC LPF for >48 kHz. <- IN PROGRESS
- Create a positive bias circuit to push the signal up from being centered at 0. <- IN PROGRESS

## Process Signal

- Interface an ESP32 with a PCM1808 for 24-bit audio using the dedicated I2S interface + drivers.
- PCM1808 already seems to apply a digital LPF.

## Write FFT

- Set up fixed-point arithmetic and test boundary cases <- IN PROGRESS
    - Configure multiple 1024-sample DMAs which feed into software buffers.
    - Convert to 1 sign bit 15 whole bits 16 fractional bits by extending padding but not performing a shift. This is equivalent to dividing by 2^16 but maintains resolution.
    - All complex multiplications are of sub-1 magnitude, so I'm not concerned about overflow through multiplication. However, because there are only 8 bits of overhead without reducing resolution, I will be sticking with a 256-sample DFT in order to avoid additive overflows.

- Try rectangular representation of complex values using two arrays for complex transformation.
- Consider sample buffer size/DFT point counts.
    - Poll to read the DMA buffers as fast as possible?
    - Just do Cooley-Tukey
    - Currently using a 256-sample DFT. In my experience, dynamic programming evaluations require less space than recursive operations, even though recursive operations may be nicer--both can be tested and evaluated.
    - Apply a Hamming window
    - Overlap every half-window size
- Test against sine wave audio, determine methods to benchmark computation speed.

- Metrics:
    - Set up and experiment with picobench

## Hook to visualizer

- Many options from LCD to electromagnet-controlled ferrofluid(!), consider more when basics are done.
