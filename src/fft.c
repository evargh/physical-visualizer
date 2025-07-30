// look up the right way to do it using C later

// for this FFT, you need to have the complex coefficient precomputed ahead of
// time by knowing the number of samples
// we can do this in fixed point instead of floating point, and benchmark 
// the difference

#include <assert.h>
#include "fft.h"

fix32_10 int32_to_fix32_10 (int32_t a) {
    return a << FRACTIONAL_BITS;
}

int32_t fix32_10_to_int32 (fix32_10 a) {
    return a >> FRACTIONAL_BITS;
}

// carry functions regardless of the decimal
fix32_10 add_fix32_10 (fix32_10 a, fix32_10 b) {
    return a + b;
}

// carry functions regardless of the decimal
fix32_10 subtract_fix32_10 (fix32_10 a, fix32_10 b) {
    return a - b;
}

// multiplying these two numbers would require 64 bits, a top 44 to represent the sign and whole part, and a bottom 20 to represent the decimal part multiplied with the other decimal part.
// perform a 64-bit multiplication and rescale down by trimming the bottom 20 and truncating the top 22
fix32_10 multiply_fix32_10 (fix32_10 a, fix32_10 b) {
   int64_t resized_a = (int64_t)(a); //64 bit
   int64_t resized_b = (int64_t)(b); //64 bit

   int64_t product = resized_a * resized_b >> FRACTIONAL_BITS;

   return (int32_t)product; 
}

// the signal being tested is an audio signal with sample rate, say, 44.1 kHz
int process_fft (int *samples, int *fft_bins, int num_samples) {
  return 0;
}

int test_arithmetic() {
  assert(int32_to_fix32_10(1) == 1024);
  assert(fix32_10_to_int32(int32_to_fix32_10(2)) == 2);
  assert(add_fix32_10(int32_to_fix32_10(1), int32_to_fix32_10(2)) == int32_to_fix32_10(3));
  assert(multiply_fix32_10(1024, 1024) == 1024);
  assert(multiply_fix32_10(int32_to_fix32_10(2), int32_to_fix32_10(3)) == int32_to_fix32_10(6));

  // more edge test cases here
  return 0;
}
