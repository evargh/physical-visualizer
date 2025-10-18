/*
 * SPDX-FileCopyrightText: 2021-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */

#include "fft.h"

#include "esp_err.h"
#include "esp_system.h"
#include "fixed_point.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include <complex.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdlib.h>

// FFT notably uses bit-reversal, which is like adding one to the MSB instead of
// the LSB Using the simple algorithm here:
// https://graphics.stanford.edu/~seander/bithacks.html#BitReverseObvious
uint32_t reverse_bits(const struct FFT_Params *params, uint32_t v) {
  unsigned int r = v; // r will be reversed bits of v; first get LSB of v
  int s = params->fft_size_bits - 1; // extra shift needed at end

  for (v >>= 1; v; v >>= 1) // shift v down until it's not zero
  {
    r <<= 1;    // do the opposite of the same operation to r
    r |= v & 1; // if v's LSB is 0, set r to 0. otherwise set it to 1
    s--;
  }
  r <<= s; // shift when v's highest bits are zero. e.g. if v had 4 non-zero
           // bits, those would be the top 4 non-zero bits of r

  r &= (1 << params->fft_size_bits) - 1; // bitmask
  return r;
}

int real_signal_to_complex(const struct FFT_Params *params,
                           const fix32_10 input[],
                           struct fix32_10_Complex output[]) {
  int fft_size = 1 << (params->fft_size_bits);

  for (size_t i = 0; i < fft_size; i++) {
    output[i] = (struct fix32_10_Complex){.real = input[i], .imag = 0};
  }
  return 0;
}

int bit_reverse_complex_array(const struct FFT_Params *params,
                              const struct fix32_10_Complex input[],
                              struct fix32_10_Complex output[]) {
  int fft_size = 1 << (params->fft_size_bits);

  // 
  for (size_t i = 0; i < fft_size; i++) {
    output[i] = input[reverse_bits(params, i)];
  }
  return 0;
}

int generate_result_freqs(const struct FFT_Params *params,
                          const struct fix32_10_Complex *complex_twiddles,
                          const fix32_10 *test_signal,
                          struct fix32_10_Complex *result_freqs) {

  int fft_size = 1 << (params->fft_size_bits);
  int fft_size_log2 = params->fft_size_bits;

  struct fix32_10_Complex *complex_signal =
      malloc(sizeof(struct fix32_10_Complex) * fft_size);
  real_signal_to_complex(params, test_signal, complex_signal);
  bit_reverse_complex_array(params, complex_signal, result_freqs);
  free(complex_signal);

  // this algorithm performs the stages of a log(N) stage FFT in-place
  for (size_t s = 1; s <= fft_size_log2; s++) {
    size_t m = 1 << s;
    size_t twiddle_index = fft_size / m;
    for (size_t k = 0; k < fft_size; k += m) {
      for (size_t j = 0; j < m / 2; j++) {
	size_t even_index = k + j;
	size_t odd_index = k + j + m/2;
        // intuitively, this follows the logic of "even + twiddle * odd"
        
	struct fix32_10_Complex even = result_freqs[even_index];
        struct fix32_10_Complex odd = result_freqs[odd_index];

	struct fix32_10_Complex even_twiddle = even_index >= (fft_size >> 1) ? multiply_fix32_10_complex((struct fix32_10_Complex){-1024, 0}, complex_twiddles[(twiddle_index * even_index) % (fft_size >> 1)]) : complex_twiddles[(twiddle_index * even_index) % (fft_size >> 1)];
	struct fix32_10_Complex odd_twiddle = odd_index >= (fft_size >> 1) ? multiply_fix32_10_complex((struct fix32_10_Complex){-1024, 0}, complex_twiddles[(twiddle_index * odd_index) % (fft_size >> 1)]) : complex_twiddles[(twiddle_index * odd_index) % (fft_size >> 1)];
	
        struct fix32_10_Complex twiddle_product = multiply_fix32_10_complex(
            even_twiddle, odd);
        
	struct fix32_10_Complex twiddle_product_complement = multiply_fix32_10_complex(
            odd_twiddle, odd);
/*
        int32_t whole_part_real = fix32_10_to_int32(twiddle_product.real);
        char fractional_part_real[FRACTIONAL_BITS + 1];
        fractional_fix32_10(twiddle_product.real, fractional_part_real);
        int32_t whole_part_imag = fix32_10_to_int32(twiddle_product.imag);
        char fractional_part_imag[FRACTIONAL_BITS + 1];
        fractional_fix32_10(twiddle_product.imag, fractional_part_imag);

        printf("%ld.%s + j%ld.%s \n", whole_part_real, fractional_part_real,
               whole_part_imag, fractional_part_imag);
*/

        result_freqs[even_index] = add_fix32_10_complex(even, twiddle_product);
        result_freqs[odd_index] =
            add_fix32_10_complex(even, twiddle_product_complement);
      }
    }
  }
  return 0;
}
