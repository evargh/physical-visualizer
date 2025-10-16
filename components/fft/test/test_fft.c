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

  uint32_t input[1 << FFT_SIZE_BITS] = {0, 1, 2,  3,  4,  5,  6,  7,
                                        8, 9, 10, 11, 12, 13, 14, 15};
  uint32_t correct[1 << FFT_SIZE_BITS] = {0, 8, 4, 12, 2, 10, 6, 14,
                                          1, 9, 5, 13, 3, 11, 7, 15};

  for (size_t i = 0; i < 1 << FFT_SIZE_BITS; i++) {
    TEST_ASSERT_EQUAL_INT(correct[i], reverse_bits(&params, input[i]));
  }
#undef FFT_SIZE
}

TEST_CASE("reverse_bits_5", "[fft]") {
#undef FFT_SIZE_BITS
#define FFT_SIZE_BITS 5

  struct FFT_Params params = {.fft_size_bits = FFT_SIZE_BITS};

  uint32_t input[1 << FFT_SIZE_BITS] = {
      0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
      16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
  uint32_t correct[1 << FFT_SIZE_BITS] = {
      0, 16, 8, 24, 4, 20, 12, 28, 2, 18, 10, 26, 6, 22, 14, 30,
      1, 17, 9, 25, 5, 21, 13, 29, 3, 19, 11, 27, 7, 23, 15, 31};

  for (size_t i = 0; i < 1 << FFT_SIZE_BITS; i++) {
    TEST_ASSERT_EQUAL_INT(correct[i], reverse_bits(&params, input[i]));
  }
#undef FFT_SIZE
}

TEST_CASE("empty_test_2", "[fft]") {
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


TEST_CASE("delta_test_2", "[fft]") {
#undef FFT_SIZE_BITS
#define FFT_SIZE_BITS 2

  struct FFT_Params params = {.fft_size_bits = FFT_SIZE_BITS};
  struct fix32_10_Complex complex_twiddle[1 << FFT_SIZE_BITS] = {
      {int32_to_fix32_10(1), 0},
      {0, int32_to_fix32_10(-1)},
      {int32_to_fix32_10(-1), 0},
      {0, int32_to_fix32_10(1)}};

  fix32_10 test_signal[1 << FFT_SIZE_BITS] = {int32_to_fix32_10(1), 0, 0, 0};

  struct fix32_10_Complex result_freqs[1 << FFT_SIZE_BITS] = {
      {0, 0}, {0, 0}, {0, 0}, {0, 0}};

  generate_result_freqs(&params, complex_twiddle, test_signal, result_freqs);

  struct fix32_10_Complex correct_freqs[1 << FFT_SIZE_BITS] = {
      {int32_to_fix32_10(1), 0}, {int32_to_fix32_10(1), 0}, {int32_to_fix32_10(1), 0}, {int32_to_fix32_10(1), 0}};

  for (size_t i = 0; i < 1 << FFT_SIZE_BITS; i++) {
    TEST_ASSERT_EQUAL_INT(correct_freqs[i].real, result_freqs[i].real);
    TEST_ASSERT_EQUAL_INT(correct_freqs[i].imag, result_freqs[i].imag);
  }
#undef FFT_SIZE
}

TEST_CASE("delta_test_3", "[fft]") {
#undef FFT_SIZE_BITS
#define FFT_SIZE_BITS 3

  struct FFT_Params params = {.fft_size_bits = FFT_SIZE_BITS};
  struct fix32_10_Complex complex_twiddle[1 << FFT_SIZE_BITS] = { 
{1024, 0}, {724, -724}, {0, -1024}, {-724, -724}, {-1024, 0}, {-724, 724}, {0, 1024}, {724, 724}
	};

  fix32_10 test_signal[1 << FFT_SIZE_BITS] = {
		int32_to_fix32_10(1), 0, 0, 0, 0, 0, 0, 0 
	};

  struct fix32_10_Complex result_freqs[1 << FFT_SIZE_BITS] = {
      {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}
	};

  generate_result_freqs(&params, complex_twiddle, test_signal, result_freqs);

  struct fix32_10_Complex correct_freqs[1 << FFT_SIZE_BITS] = {
      {int32_to_fix32_10(1), 0}, {int32_to_fix32_10(1), 0}, {int32_to_fix32_10(1), 0}, {int32_to_fix32_10(1), 0},
      {int32_to_fix32_10(1), 0}, {int32_to_fix32_10(1), 0}, {int32_to_fix32_10(1), 0}, {int32_to_fix32_10(1), 0}
	};

  for (size_t i = 0; i < 1 << FFT_SIZE_BITS; i++) {
    // expecting 1. A close 5% approximation in decimal is about 0 00000000000000000000.0000110011
    // this is 51
    TEST_ASSERT_INT_WITHIN(51, correct_freqs[i].real, result_freqs[i].real);
    TEST_ASSERT_INT_WITHIN(51, correct_freqs[i].imag, result_freqs[i].imag);
  }
#undef FFT_SIZE
}

TEST_CASE("delta_test_5", "[fft]") {
#undef FFT_SIZE_BITS
#define FFT_SIZE_BITS 5

  struct FFT_Params params = {.fft_size_bits = FFT_SIZE_BITS};
  struct fix32_10_Complex complex_twiddle[1 << FFT_SIZE_BITS] = {
{1024, 0}, {1004, -200}, {946, -392}, {851, -569}, {724, -724}, {569, -851}, {392, -946}, {200, -1004}, {0, -1024}, {-200, -1004}, {-392, -946}, {-569, -851}, {-724, -724}, {-851, -569}, {-946, -392}, {-1004, -200}, {-1024, 0}, {-1004, 200}, {-946, 392}, {-851, 569}, {-724, 724}, {-569, 851}, {-392, 946}, {-200, 1004}, {0, 1024}, {200, 1004}, {392, 946}, {569, 851}, {724, 724}, {851, 569}, {946, 392}, {1004, 200}
	};

  fix32_10 test_signal[1 << FFT_SIZE_BITS] = {
		int32_to_fix32_10(1), 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0
	};

  struct fix32_10_Complex result_freqs[1 << FFT_SIZE_BITS] = {
      {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
      {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
      {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
      {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}
	};

  generate_result_freqs(&params, complex_twiddle, test_signal, result_freqs);

  struct fix32_10_Complex correct_freqs[1 << FFT_SIZE_BITS] = {
      {int32_to_fix32_10(1), 0}, {int32_to_fix32_10(1), 0}, {int32_to_fix32_10(1), 0}, {int32_to_fix32_10(1), 0},
      {int32_to_fix32_10(1), 0}, {int32_to_fix32_10(1), 0}, {int32_to_fix32_10(1), 0}, {int32_to_fix32_10(1), 0},
      {int32_to_fix32_10(1), 0}, {int32_to_fix32_10(1), 0}, {int32_to_fix32_10(1), 0}, {int32_to_fix32_10(1), 0},
      {int32_to_fix32_10(1), 0}, {int32_to_fix32_10(1), 0}, {int32_to_fix32_10(1), 0}, {int32_to_fix32_10(1), 0},
      {int32_to_fix32_10(1), 0}, {int32_to_fix32_10(1), 0}, {int32_to_fix32_10(1), 0}, {int32_to_fix32_10(1), 0},
      {int32_to_fix32_10(1), 0}, {int32_to_fix32_10(1), 0}, {int32_to_fix32_10(1), 0}, {int32_to_fix32_10(1), 0},
      {int32_to_fix32_10(1), 0}, {int32_to_fix32_10(1), 0}, {int32_to_fix32_10(1), 0}, {int32_to_fix32_10(1), 0},
      {int32_to_fix32_10(1), 0}, {int32_to_fix32_10(1), 0}, {int32_to_fix32_10(1), 0}, {int32_to_fix32_10(1), 0}
	};

  for (size_t i = 0; i < 1 << FFT_SIZE_BITS; i++) {
    // expecting 1. A close 5% approximation in decimal is about 0 00000000000000000000.0000110011
    // this is 51
    TEST_ASSERT_INT_WITHIN(51, correct_freqs[i].real, result_freqs[i].real);
    TEST_ASSERT_INT_WITHIN(51, correct_freqs[i].imag, result_freqs[i].imag);
  }
#undef FFT_SIZE
}
