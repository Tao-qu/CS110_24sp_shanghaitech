#include "FloatCalculate.h"
#include <stdbool.h>

const size_t SIGN_BIT = 1;
const size_t EXPONENT_BITS = 8;
const size_t MANTISSA_BITS = 23;
const size_t EXTRA_BITS = 3;

// static int32_t get_norm_bias(void) { return 1 - (1 << (EXPONENT_BITS - 1)); }

// static int32_t get_denorm_bias(void) { return 1 + get_norm_bias(); }

static bool test_rightmost_all_zeros(uint32_t number, size_t bits)
{
  uint32_t mask = (1ull << bits) - 1;
  return (number & mask) == 0;
}

static bool test_rightmost_all_ones(uint32_t number, size_t bits)
{
  uint32_t mask = (1ull << bits) - 1;
  return (number & mask) == mask;
}

// You can also design a function of your own.
static void build_bitstring(Float input, char *output)
{
  // Sign bit
  output[0] = input.sign ? '1' : '0';

  // Exponent bits
  for (size_t i = 1; i <= EXPONENT_BITS; ++i)
  {
    output[i] = (input.exponent >> (EXPONENT_BITS - i)) & 1 ? '1' : '0';
  }

  // Mantissa bits
  for (size_t i = EXPONENT_BITS + 1; i <= EXPONENT_BITS + MANTISSA_BITS; ++i)
  {
    output[i] = (input.mantissa >> (EXPONENT_BITS + MANTISSA_BITS - i)) & 1 ? '1' : '0';
  }

  output[EXPONENT_BITS + MANTISSA_BITS + 1] = '\0';
}

// You can also design a function of your own.
static Float parse_bitstring(const char *input)
{
  // Initialize the result
  Float result;
  result.sign = input[0] == '1'; // Sign bit
  result.exponent = 0;
  result.mantissa = 0;

  // Exponent bits
  for (size_t i = 1; i <= EXPONENT_BITS; ++i)
  {
    result.exponent = (result.exponent << 1) + (input[i] == '1');
  }

  // Mantissa bits
  for (size_t i = EXPONENT_BITS + 1; i <= EXPONENT_BITS + MANTISSA_BITS; ++i)
  {
    result.mantissa = (result.mantissa << 1) + (input[i] == '1');
  }

  // Determine the type of the floating-point number
  if (test_rightmost_all_zeros(result.exponent, EXPONENT_BITS))
  {
    if (test_rightmost_all_zeros(result.mantissa, MANTISSA_BITS))
    {
      result.type = ZERO_T;
    }
    else
    {
      result.type = DENORMALIZED_T;
    }
  }
  else if (test_rightmost_all_ones(result.exponent, EXPONENT_BITS))
  {
    if (test_rightmost_all_zeros(result.mantissa, MANTISSA_BITS))
    {
      result.type = INFINITY_T;
    }
    else
    {
      result.type = NAN_T;
    }
  }
  else
  {
    result.type = NORMALIZED_T;
  }

  return result;
}

// You can also design a function of your own.
static Float float_add_impl(Float a, Float b)
{
  // Add the hidden '1' and the extra bits to the mantissa
  if (a.type != DENORMALIZED_T)
  {
    a.mantissa = (a.mantissa | (1ull << MANTISSA_BITS)) << EXTRA_BITS;
  }
  else
  {
    a.mantissa <<= EXTRA_BITS;
    ++a.exponent;
  }
  if (b.type != DENORMALIZED_T)
  {
    b.mantissa = (b.mantissa | (1ull << MANTISSA_BITS)) << EXTRA_BITS;
  }
  else
  {
    b.mantissa <<= EXTRA_BITS;
    ++b.exponent;
  }

  // Align the exponents
  if (a.exponent < b.exponent)
  {
    size_t sticky_flag = 0;
    for (size_t i = 0; i < b.exponent - a.exponent; ++i)
    {
      if (test_rightmost_all_ones(a.mantissa, 1) && sticky_flag == 0)
      {
        ++sticky_flag;
      }
      a.mantissa >>= 1;
    }
    a.mantissa |= sticky_flag;
    a.exponent = b.exponent;
  }
  else
  {
    size_t sticky_flag = 0;
    for (size_t i = 0; i < a.exponent - b.exponent; ++i)
    {
      if (test_rightmost_all_ones(b.mantissa, 1) && sticky_flag == 0)
      {
        ++sticky_flag;
      }
      b.mantissa >>= 1;
    }
    b.mantissa |= sticky_flag;
  }

  // Add or subtract the mantissas
  if (a.sign != b.sign)
  {
    if (a.mantissa < b.mantissa)
    {
      a.sign = b.sign;
      a.mantissa = b.mantissa - a.mantissa;
    }
    else
    {
      a.mantissa -= b.mantissa;
    }
  }
  else
  {
    a.mantissa += b.mantissa;
  }

  // Normalize the result
  while (a.mantissa >= 1ull << (MANTISSA_BITS + EXTRA_BITS + 1))
  {
    if (a.exponent != (1ull << EXPONENT_BITS) - 1)
    {
      a.mantissa >>= 1;
      ++a.exponent;
    }
    else
    {
      a.mantissa = 0;
      break;
    }
  }
  while (a.mantissa < 1ull << (MANTISSA_BITS + EXTRA_BITS))
  {
    if (a.exponent != 0)
    {
      a.mantissa <<= 1;
      --a.exponent;
    }
    else
    {
      break;
    }
  }
  if (a.exponent == 0)
  {
    a.mantissa >>= 1;
  }
  a.mantissa >>= EXTRA_BITS;
  a.mantissa &= (1ull << MANTISSA_BITS) - 1;

  // Determine the type of the result
  if (test_rightmost_all_zeros(a.exponent, EXPONENT_BITS))
  {
    if (test_rightmost_all_zeros(a.mantissa, MANTISSA_BITS))
    {
      a.type = ZERO_T;
    }
    else
    {
      a.type = DENORMALIZED_T;
    }
  }
  else if (test_rightmost_all_ones(a.exponent, EXPONENT_BITS))
  {
    if (test_rightmost_all_zeros(a.mantissa, MANTISSA_BITS))
    {
      a.type = INFINITY_T;
    }
    else
    {
      a.type = NAN_T;
    }
  }
  else
  {
    a.type = NORMALIZED_T;
  }
  return a;
}

// You should not modify the signature of this function
void float_add(const char *a, const char *b, char *result)
{
  // TODO: Implement this function
  // A possible implementation of the function:
  Float fa = parse_bitstring(a);
  Float fb = parse_bitstring(b);
  Float fresult = float_add_impl(fa, fb);
  build_bitstring(fresult, result);
}
