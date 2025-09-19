#include "fixed_point.h"

struct FFT_Params {
  int fft_size_bits;
};

uint32_t reverse_bits(const struct FFT_Params *params, uint32_t v);

int real_signal_to_complex(const struct FFT_Params *params,
                           const fix32_10 input[],
                           struct fix32_10_Complex output[]);

int bit_reverse_complex_array(const struct FFT_Params *params,
                              const struct fix32_10_Complex input[],
                              struct fix32_10_Complex output[]);

int generate_result_freqs(const struct FFT_Params *params,
                          struct fix32_10_Complex *complex_twiddles,
                          const fix32_10 *test_signal,
                          struct fix32_10_Complex *result_freqs);
