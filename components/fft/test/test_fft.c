#include "fft.h" // Include the header for the module being tested
#include "fixed_point.h"
#include "unity.h" // Include the Unity test framework header
#include <stdint.h>

TEST_CASE("reverse_bits_2", "[fft]") {
#undef FFT_SIZE_BITS
#define FFT_SIZE_BITS 2

  struct FFT_Params params = {.fft_size_bits = FFT_SIZE_BITS};

  uint32_t input[1 << FFT_SIZE_BITS] = {0, 1, 2, 3};
  uint32_t correct[1 << FFT_SIZE_BITS] = {0, 2, 1, 3};

  for (size_t i = 0; i < 1 << FFT_SIZE_BITS; i++) {
    TEST_ASSERT_EQUAL_INT(correct[i], reverse_bits(&params, input[i]));
  }
#undef FFT_SIZE
}


TEST_CASE("reverse_bits_3", "[fft]") {
#undef FFT_SIZE_BITS
#define FFT_SIZE_BITS 3

  struct FFT_Params params = {.fft_size_bits = FFT_SIZE_BITS};

  uint32_t input[1 << FFT_SIZE_BITS] = {0, 1, 2, 3, 4, 5, 6, 7};
  uint32_t correct[1 << FFT_SIZE_BITS] = {0, 4, 2, 6, 1, 5, 3, 7};

  for (size_t i = 0; i < 1 << FFT_SIZE_BITS; i++) {
    TEST_ASSERT_EQUAL_INT(correct[i], reverse_bits(&params, input[i]));
  }
#undef FFT_SIZE
}


TEST_CASE("reverse_bits_4", "[fft]") {
#undef FFT_SIZE_BITS
#define FFT_SIZE_BITS 4

  struct FFT_Params params = {.fft_size_bits = FFT_SIZE_BITS};

  uint32_t input[1 << FFT_SIZE_BITS] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
  uint32_t correct[1 << FFT_SIZE_BITS] = {0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};

  for (size_t i = 0; i < 1 << FFT_SIZE_BITS; i++) {
    TEST_ASSERT_EQUAL_INT(correct[i], reverse_bits(&params, input[i]));
  }
#undef FFT_SIZE
}

TEST_CASE("empty_test", "[fft]") {
#undef FFT_SIZE_BITS
#define FFT_SIZE_BITS 2

  struct FFT_Params params = {.fft_size_bits = FFT_SIZE_BITS};
  struct fix32_10_Complex complex_twiddle[1 << FFT_SIZE_BITS] = {
      {int32_to_fix32_10(1), 0},
      {0, int32_to_fix32_10(-1)},
      {int32_to_fix32_10(-1), 0},
      {0, int32_to_fix32_10(1)}};

  fix32_10 test_signal[1 << FFT_SIZE_BITS] = {0, 0, 0, 0};

  struct fix32_10_Complex result_freqs[1 << FFT_SIZE_BITS] = {
      {0, 0}, {0, 0}, {0, 0}, {0, 0}};

  generate_result_freqs(&params, complex_twiddle, test_signal, result_freqs);

  struct fix32_10_Complex correct_freqs[1 << FFT_SIZE_BITS] = {
      {0, 0}, {0, 0}, {0, 0}, {0, 0}};

  for (size_t i = 0; i < 1 << FFT_SIZE_BITS; i++) {
    TEST_ASSERT_EQUAL_INT(correct_freqs[i].real, result_freqs[i].real);
    TEST_ASSERT_EQUAL_INT(correct_freqs[i].imag, result_freqs[i].imag);
  }
#undef FFT_SIZE
}
