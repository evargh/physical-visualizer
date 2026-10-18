#ifndef FIXED_POINT_H
#define FIXED_POINT_H

#include <stdint.h>
#include "esp_err.h"

// Note that the Q being at 10 in a 32-bit number means that the maximum value that can be realized is set to the positive overflow point. A 1024-value FFT would be, worst case, a bit shift left by 10. As a result, all read values should be 12-bit integers
#define FRACTIONAL_BITS 10
// if a number is equal to 2**(32-FRACTIONAL BITS), the upward shift would result in a negative number
#define POSITIVE_OVERFLOW_POINT ((1 << (32 - FRACTIONAL_BITS - 1)) - 1)
// similarly, over this point the shift would become positive
#define NEGATIVE_OVERFLOW_POINT (-(1 << (32 - FRACTIONAL_BITS - 1)))

typedef int32_t fix32_10;

struct fix32_10_Complex {
  fix32_10 real;
  fix32_10 imag;
};

fix32_10 int32_to_fix32_10(int32_t a);
int32_t fix32_10_to_int32(fix32_10 a);

void fractional_fix32_10(fix32_10 a, char string[FRACTIONAL_BITS+1]);

fix32_10 add_fix32_10(fix32_10 a, fix32_10 b);
fix32_10 subtract_fix32_10(fix32_10 a, fix32_10 b);
fix32_10 multiply_fix32_10(fix32_10 a, fix32_10 b);

struct fix32_10_Complex add_fix32_10_complex(struct fix32_10_Complex a, struct fix32_10_Complex b);
struct fix32_10_Complex subtract_fix32_10_complex(struct fix32_10_Complex a, struct fix32_10_Complex b);
struct fix32_10_Complex multiply_fix32_10_complex(struct fix32_10_Complex a, struct fix32_10_Complex b);
struct fix32_10_Complex get_fix32_10_complex_conjugate(struct fix32_10_Complex a);
fix32_10 get_fix32_10_complex_square_magnitude(struct fix32_10_Complex a);

#endif
