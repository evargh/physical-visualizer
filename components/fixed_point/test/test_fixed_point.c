#include "unity.h" // Include the Unity test framework header
#include "fixed_point.h" // Include the header for the module being tested

TEST_CASE("int32_to_fix32_10 converts 1 to 1024", "[fixed_point]")
{
    fix32_10 result = int32_to_fix32_10(1);
    TEST_ASSERT_EQUAL_INT(1024, result);
}

TEST_CASE("fix32_10_to_int32 converts 1024 to 1", "[fixed_point]")
{
    int32_t result = fix32_10_to_int32(1024);
    TEST_ASSERT_EQUAL_INT(1, result);
}

TEST_CASE("int32_to_fix32_10 converts -1 to -1024", "[fixed_point]")
{
    fix32_10 result = int32_to_fix32_10(-1);
    TEST_ASSERT_EQUAL_INT(-1024, result);
}

TEST_CASE("fix32_10_to_int32 converts -1024 to -1", "[fixed_point]")
{
    int32_t result = fix32_10_to_int32(-1024);
    TEST_ASSERT_EQUAL_INT(-1, result);
}

TEST_CASE("Round-trip conversion of +2", "[fixed_point]")
{
    int32_t result = fix32_10_to_int32(int32_to_fix32_10(2));
    TEST_ASSERT_EQUAL_INT(2, result);
}

TEST_CASE("Round-trip conversion of -2", "[fixed_point]")
{
    int32_t result = fix32_10_to_int32(int32_to_fix32_10(-2));
    TEST_ASSERT_EQUAL_INT(-2, result);
}

TEST_CASE("Addition: 1 + 2 == 3", "[fixed_point]")
{
    fix32_10 result = add_fix32_10(int32_to_fix32_10(1), int32_to_fix32_10(2));
    TEST_ASSERT_EQUAL_INT(int32_to_fix32_10(3), result);
}

TEST_CASE("Addition: -1 + 2 == 1", "[fixed_point]")
{
    fix32_10 result = add_fix32_10(int32_to_fix32_10(-1), int32_to_fix32_10(2));
    TEST_ASSERT_EQUAL_INT(int32_to_fix32_10(1), result);
}

TEST_CASE("Multiplication: 1024 * 1024 == 1024", "[fixed_point]")
{
    fix32_10 result = multiply_fix32_10(1024, 1024);
    TEST_ASSERT_EQUAL_INT(1024, result);
}

TEST_CASE("Multiplication: 1024 * -1024 == -1024", "[fixed_point]")
{
    fix32_10 result = multiply_fix32_10(1024, -1024);
    TEST_ASSERT_EQUAL_INT(-1024, result);
}

TEST_CASE("Multiplication: 2 * 3 == 6", "[fixed_point]")
{
    fix32_10 result = multiply_fix32_10(int32_to_fix32_10(2), int32_to_fix32_10(3));
    TEST_ASSERT_EQUAL_INT(int32_to_fix32_10(6), result);
}

TEST_CASE("Multiplication: 2 * -3 == -6", "[fixed_point]")
{
    fix32_10 result = multiply_fix32_10(int32_to_fix32_10(2), int32_to_fix32_10(-3));
    TEST_ASSERT_EQUAL_INT(int32_to_fix32_10(-6), result);
}

TEST_CASE("Multiplication: 135 * -102 == -13770", "[fixed_point]")
{
    fix32_10 result = multiply_fix32_10(int32_to_fix32_10(135), int32_to_fix32_10(-102));
    TEST_ASSERT_EQUAL_INT(int32_to_fix32_10(-13770), result);
}

TEST_CASE("Fixed-point conversion of 0", "[fixed_point]")
{
    fix32_10 result = int32_to_fix32_10(0);
    TEST_ASSERT_EQUAL_INT(0, result);
}

TEST_CASE("Fixed-point conversion of max positive int 2097151", "[fixed_point]")
{
    fix32_10 result = int32_to_fix32_10(2097151);
    TEST_ASSERT_EQUAL_INT(0x7FFFFC00, result); // 2097151 << 10
}

TEST_CASE("Fixed-point conversion of min negative int -2097152", "[fixed_point]")
{
    fix32_10 result = int32_to_fix32_10(-2097152);
    TEST_ASSERT_EQUAL_INT(0x80000000, result); // INT32_MIN
}

/* no great way to test assertions here
TEST_CASE("Addition overflow: max + 1", "[fixed_point]")
{
    fix32_10 a = int32_to_fix32_10(2097151);
    fix32_10 b = int32_to_fix32_10(1);
    fix32_10 result = add_fix32_10(a, b);
    // Expected to assert
}
*/

TEST_CASE("Subtraction: 5 - 3 == 2", "[fixed_point]")
{
    fix32_10 result = subtract_fix32_10(int32_to_fix32_10(5), int32_to_fix32_10(3));
    TEST_ASSERT_EQUAL_INT(int32_to_fix32_10(2), result);
}

TEST_CASE("Subtraction: -3 - 5 == -8", "[fixed_point]")
{
    fix32_10 result = subtract_fix32_10(int32_to_fix32_10(-3), int32_to_fix32_10(5));
    TEST_ASSERT_EQUAL_INT(int32_to_fix32_10(-8), result);
}

TEST_CASE("Multiplication with fractional part: 1.5 * 2 == 3", "[fixed_point]")
{
    fix32_10 a = int32_to_fix32_10(1) + 512; // 1.5 in this datatype
    fix32_10 b = int32_to_fix32_10(2);
    fix32_10 result = multiply_fix32_10(a, b);
    TEST_ASSERT_EQUAL_INT(int32_to_fix32_10(3), result);
}

TEST_CASE("Multiplication: -2 * -3 == 6", "[fixed_point]")
{
    fix32_10 result = multiply_fix32_10(int32_to_fix32_10(-2), int32_to_fix32_10(-3));
    TEST_ASSERT_EQUAL_INT(int32_to_fix32_10(6), result);
}

/*******
* fix32_10_Complex Testing
* ******/

TEST_CASE("Complex Addition: (1 + 0j) + (1 + 0j) = 2 + 0j", "[fixed_point]")
{
    struct fix32_10_Complex a = {int32_to_fix32_10(1), int32_to_fix32_10(0)};
    struct fix32_10_Complex b = {int32_to_fix32_10(1), int32_to_fix32_10(0)};
    struct fix32_10_Complex result = add_fix32_10_complex(a, b);
    TEST_ASSERT_EQUAL_INT(result.real, int32_to_fix32_10(2));
    TEST_ASSERT_EQUAL_INT(result.imag, int32_to_fix32_10(0));
}

TEST_CASE("Complex Addition: (0 + 1j) + (0 + 1j) = 0 + 2j", "[fixed_point]")
{
    struct fix32_10_Complex a = {int32_to_fix32_10(0), int32_to_fix32_10(1)};
    struct fix32_10_Complex b = {int32_to_fix32_10(0), int32_to_fix32_10(1)};
    struct fix32_10_Complex result = add_fix32_10_complex(a, b);
    TEST_ASSERT_EQUAL_INT(result.real, int32_to_fix32_10(0));
    TEST_ASSERT_EQUAL_INT(result.imag, int32_to_fix32_10(2));
}

TEST_CASE("Complex Subtraction: (1 + 0j) - (1 + 0j) = 0 + 0j", "[fixed_point]")
{
    struct fix32_10_Complex a = {int32_to_fix32_10(1), int32_to_fix32_10(0)};
    struct fix32_10_Complex b = {int32_to_fix32_10(1), int32_to_fix32_10(0)};
    struct fix32_10_Complex result = subtract_fix32_10_complex(a, b);
    TEST_ASSERT_EQUAL_INT(result.real, int32_to_fix32_10(0));
    TEST_ASSERT_EQUAL_INT(result.imag, int32_to_fix32_10(0));
}

TEST_CASE("Complex Subtraction: (0 + 1j) - (0 + 1j) = 0 + 0j", "[fixed_point]")
{
    struct fix32_10_Complex a = {int32_to_fix32_10(0), int32_to_fix32_10(1)};
    struct fix32_10_Complex b = {int32_to_fix32_10(0), int32_to_fix32_10(1)};
    struct fix32_10_Complex result = subtract_fix32_10_complex(a, b);
    TEST_ASSERT_EQUAL_INT(result.real, int32_to_fix32_10(0));
    TEST_ASSERT_EQUAL_INT(result.imag, int32_to_fix32_10(0));
}

TEST_CASE("Complex Multiplication: (0 + 1j) * (0 + 1j) = -1 + 0j", "[fixed_point]")
{
    struct fix32_10_Complex a = {int32_to_fix32_10(0), int32_to_fix32_10(1)};
    struct fix32_10_Complex b = {int32_to_fix32_10(0), int32_to_fix32_10(1)};
    struct fix32_10_Complex result = multiply_fix32_10_complex(a, b);
    TEST_ASSERT_EQUAL_INT(result.real, int32_to_fix32_10(-1));
    TEST_ASSERT_EQUAL_INT(result.imag, int32_to_fix32_10(0));
}

TEST_CASE("Complex Multiplication: (2 + 3j) * (5 + 9j) = -17 + 33j", "[fixed_point]")
{
    struct fix32_10_Complex a = {int32_to_fix32_10(2), int32_to_fix32_10(3)};
    struct fix32_10_Complex b = {int32_to_fix32_10(5), int32_to_fix32_10(9)};
    struct fix32_10_Complex result = multiply_fix32_10_complex(a, b);
    TEST_ASSERT_EQUAL_INT(result.real, int32_to_fix32_10(-17));
    TEST_ASSERT_EQUAL_INT(result.imag, int32_to_fix32_10(33));
}
