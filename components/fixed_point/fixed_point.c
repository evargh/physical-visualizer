#include "fixed_point.h"
#include <assert.h>

#define ASSERT_OR_FAIL(condition) \
    if (!(condition)) { \
        TEST_FAIL_MESSAGE("Assertion failed: " #condition); \
    }

fix32_10 int32_to_fix32_10(int32_t a) {
  assert(a <= POSITIVE_OVERFLOW_POINT && a >= NEGATIVE_OVERFLOW_POINT);
  return a << FRACTIONAL_BITS;
}

int32_t fix32_10_to_int32(fix32_10 a) {
  // if a was negative, the FRACTIONAL_BITS MSBs should be 1s
  if (a < 0) {
    return (
	(a >> FRACTIONAL_BITS) | (((1 << FRACTIONAL_BITS) - 1) << (32 - FRACTIONAL_BITS))
	);
  } else {
    return a >> FRACTIONAL_BITS;
  }
}

void fractional_fix32_10(fix32_10 a, char string[FRACTIONAL_BITS+1]) {
	// to turn this into a whole number, return the decimal number of the fractional part multiplied with 2**(-10) i.e. 9765625. There are 10 digits in the decimal part maximum, so add leading zeroes as necessary
    uint32_t mask = (1 << FRACTIONAL_BITS) - 1;
    uint32_t fractional_part = abs(a) & mask;
    int64_t fractional_value = (int64_t)fractional_part * 9765625;

    snprintf(string, FRACTIONAL_BITS+1, "%010lld", fractional_value);
}

// carry functions regardless of the decimal
fix32_10 add_fix32_10(fix32_10 a, fix32_10 b) { 
	int64_t sum = (int64_t)a + (int64_t)b;
	if(!(sum < INT32_MAX && sum > INT32_MIN)) {
		printf("sum failure, %ld %ld\n", (int32_t)a, (int32_t)b);
		assert(sum < INT32_MAX && sum > INT32_MIN);
	}
	return (fix32_10)sum; 
}

// carry functions regardless of the decimal
fix32_10 subtract_fix32_10(fix32_10 a, fix32_10 b) {
	int64_t sum = (int64_t)a - (int64_t)b;
	if(!(sum < INT32_MAX && sum > INT32_MIN)) {
		printf("subtraction failure, %ld %ld\n", (int32_t)a, (int32_t)b);
		assert(sum < INT32_MAX && sum > INT32_MIN);
	}
	return (fix32_10)sum; 
}

// multiplying these two numbers would require 64 bits, a top 44 to represent
// the sign and whole part, and a bottom 20 to represent the decimal part
// multiplied with the other decimal part. perform a 64-bit multiplication and
// rescale down by trimming the bottom 10 and truncating the top 22
//
// TODO: is there a standard way to announce t
fix32_10 multiply_fix32_10(fix32_10 a, fix32_10 b) {
  int64_t resized_a = (int64_t)(a); // 64 bit
  int64_t resized_b = (int64_t)(b); // 64 bit

  int64_t product = (resized_a * resized_b) >> FRACTIONAL_BITS;

  if(!(product < INT32_MAX && product > INT32_MIN)) {
	printf("multiplication failure, %ld %ld\n", (int32_t)a, (int32_t)b);
	assert(product < INT32_MAX && product > INT32_MIN);
  }

  return (fix32_10)product;
}

struct fix32_10_Complex add_fix32_10_complex(struct fix32_10_Complex a, struct fix32_10_Complex b) {
  return (struct fix32_10_Complex){
      .real = add_fix32_10(a.real, b.real),
      .imag = add_fix32_10(a.imag, b.imag)};
}

struct fix32_10_Complex subtract_fix32_10_complex(struct fix32_10_Complex a, struct fix32_10_Complex b) {
  return (struct fix32_10_Complex){
      .real = subtract_fix32_10(a.real, b.real),
      .imag = subtract_fix32_10(a.imag, b.imag)};
}

struct fix32_10_Complex multiply_fix32_10_complex(struct fix32_10_Complex a, struct fix32_10_Complex b) {
  return (struct fix32_10_Complex){
      .real = subtract_fix32_10(multiply_fix32_10(a.real, b.real),
                           multiply_fix32_10(a.imag, b.imag)),
      .imag = add_fix32_10(multiply_fix32_10(a.real, b.imag),
                           multiply_fix32_10(a.imag, b.real))};
}

struct fix32_10_Complex get_fix32_10_complex_conjugate(struct fix32_10_Complex a) {
  return (struct fix32_10_Complex) {.real = a.real, .imag = multiply_fix32_10(int32_to_fix32_10(-1), a.imag)};
}

//TODO: Fix the off-by-one error that generates an imaginary part
fix32_10 get_fix32_10_complex_square_magnitude(struct fix32_10_Complex a) {
  struct fix32_10_Complex answer = multiply_fix32_10_complex(a, get_fix32_10_complex_conjugate(a));
  if(!(answer.imag == 0)) {
    printf("Magnitude produced imaginary component %ld from %ld + j*%ld\n", (int32_t)answer.imag, (int32_t)a.real, (int32_t)a.imag);
    assert(answer.imag == 0);
  }
  return answer.real; 
}
