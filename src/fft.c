// look up the right way to do it using C later

// for this FFT, you need to have the complex coefficient precomputed ahead of
// time by knowing the number of samples
// we can do this in fixed point instead of floating point, and benchmark the difference
//
// our fixed-point format can be q8.8, with a sign bit

#include <assert.h>
#include "fft.h"

const int FRACTIONAL_BITS = 8;

// carry functions regardless of the decimal
int add_8_8_fixed (int a, int b) {
    return a + b;
}

// carry functions regardless of the decimal
int subtract_8_8_fixed (int a, int b) {
    return a - b;
}

// the signal being tested is an audio signal with sample rate, say, 44.1 kHz
int process_fft (int *samples, int *fft_bins, int num_samples) {
  return 0;
}

int test_arithmetic() {
  assert(add_8_8_fixed(1,2) == 3);
  // more edge test cases here
  return 0;
}
