#ifndef FFT_H
#define FFT_H

#include <stdint.h>

#define FRACTIONAL_BITS 10
#define SAMPLE_BUFFER_SIZE 256

typedef int32_t fix32_10;

fix32_10 int32_to_fix32_10(int32_t a);
int32_t fix32_10_to_int32(fix32_10 a);

fix32_10 add_fix32_10(fix32_10 a, fix32_10 b);
fix32_10 subtract_fix32_10(fix32_10 a, fix32_10 b);
fix32_10 multiply_fix32_10(fix32_10 a, fix32_10 b);

uint16_t populate_sample_buffer(int32_t a, uint16_t pointer);

int process_fft(int *fft_bins, int num_samples);

int test_arithmetic();

// circular buffer to quickly store entries
extern int process_flag;

#endif
