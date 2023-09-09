#include "./s21_decimal.h"

int s21_div_int(const unsigned *value, unsigned *result, unsigned divider,
                int size) {
  if (divider != 0) {
    unsigned long long buf = 0ull;
    unsigned long long div_buf = 0ull;
    unsigned mod = 0;
    for (int i = size - 1; i >= 0; i--) {
      mod = ((unsigned long long)buf + (unsigned long long)value[i]) %
            (unsigned long long)divider;
      div_buf = ((unsigned long long)buf + (unsigned long long)value[i]) /
                (unsigned long long)divider;
      result[i] = (unsigned)div_buf;
      buf = mod;
      buf = buf << 32;
    }
  }
  return !!divider;
}

int s21_sign_compare(s21_decimal value1, s21_decimal value2) {
  unsigned sign = 0u;
  if (s21_get_sign(value1) > s21_get_sign(value2)) {
    sign = 1;
  } else if (s21_get_sign(value2) > s21_get_sign(value1)) {
    sign = -1;
  }
  return sign;
}

void s21_shift_scale(s21_decimal value1, s21_decimal value2, unsigned *buff1,
                     unsigned *buff2, int size) {
  unsigned scale1 = s21_get_scale(value1);
  unsigned scale2 = s21_get_scale(value2);
  unsigned scale = scale1 > scale2 ? scale1 : scale2;
  s21_to_scale_mem(value1, buff1, scale, size);
  s21_to_scale_mem(value2, buff2, scale, size);
}

void s21_to_scale_mem(s21_decimal value, unsigned *buff, unsigned scale,
                      int size) {
  unsigned shift = scale - s21_get_scale(value);
  s21_clear_bits(buff, size);
  memcpy(buff, value.bits, 3 * sizeof(unsigned));
  for (unsigned i = 0; i < shift; i++) {
    s21_mul_int(buff, buff, 10, size);
  }
}

int s21_is_equal_mem(const unsigned *value1, const unsigned *value2, int size) {
  int flag = 1;
  for (int i = size - 1; i >= 0; i--) {
    flag = flag & (value1[i] == value2[i]);
  }
  return flag;
}

int s21_is_greater_mem(const unsigned *value1, const unsigned *value2,
                       int size) {
  int flag = 0;
  int result = 0;
  for (int i = size - 1; (i >= 0) & (!flag); i--) {
    if (value1[i] > value2[i]) {
      flag = 1;
      result = 1;
    } else if (value1[i] < value2[i]) {
      flag = 1;
      result = 0;
    }
  }
  return result;
}

int s21_mem_add(const unsigned *value1, const unsigned *value2,
                unsigned *result, int size) {
  unsigned long long buf = 0ull;
  for (int i = 0; i < size; i++) {
    buf += (unsigned long long)value1[i] + (unsigned long long)value2[i];
    result[i] = (unsigned)buf;
    buf = buf >> 32;
  }
  return !!buf;
}

int s21_mod_int(const unsigned *value, unsigned divider, int size) {
  unsigned mod = 0;
  if (divider != 0) {
    unsigned long long buf = 0ull;
    for (int i = size - 1; i >= 0; i--) {
      mod =
          (unsigned)(((unsigned long long)buf + (unsigned long long)value[i]) %
                     (unsigned long long)divider);
      buf = (unsigned long long)mod;
      buf = buf << 32;
    }
  }
  return mod;
}

int s21_mul_int(const unsigned *value, unsigned *result, unsigned multiplier,
                int size) {
  unsigned long long int addtiply = 0ull;
  for (int i = 0; i < size; i++) {
    addtiply = addtiply +
               ((unsigned long long)value[i] * (unsigned long long)multiplier);
    result[i] = (unsigned)addtiply;
    addtiply = addtiply >> 32;
  }
  return (addtiply > 0);
}

int s21_add_int(const unsigned *value, unsigned *result, unsigned addtiplier,
                int size) {
  unsigned long long int addtiply = (unsigned long long)addtiplier;
  for (int i = 0; i < size; i++) {
    addtiply = addtiply + (unsigned long long)value[i];
    result[i] = (unsigned)addtiply;
    addtiply = addtiply >> 32;
  }
  return (addtiply > 0);
}

int s21_negate_mem(const unsigned *value, unsigned *result, int size) {
  for (int i = 0; i < size; i++) {
    result[i] = ~value[i];
  }
  s21_add_int(result, result, 1, size);
  return 0;
}

int s21_set_sign(s21_decimal *value, int x) {
  if (x) {
    value->bits[3] |= 0b10000000000000000000000000000000;
  } else {
    value->bits[3] &= 0b01111111111111111111111111111111;
  }
  return 0;
}

void s21_clear_bits(unsigned *result, int size) {
  for (int i = 0; i < size; i++) {
    result[i] = 0;
  }
}

int s21_get_sign(const s21_decimal value) {
  int sign = value.bits[3] >> 31 & 1;
  return !!sign;
}

int s21_get_scale(const s21_decimal value) {
  return (unsigned char)(value.bits[3] >> 16);
}
void s21_set_scale(s21_decimal *value, int scale) {
  value->bits[3] =
      (value->bits[3] & 0xFF00FFFF) | (((unsigned)scale & 0xFF) << 16);
  value->bits[3] =
      (value->bits[3] & 0xFF00FFFF) | (((unsigned)scale & 0xFF) << 16);
}

int s21_dec_is_zero(s21_decimal value) {
  return !(value.bits[0] || value.bits[1] || value.bits[2]);
}

void s21_copy_dec_to_dec(const s21_decimal *value, s21_decimal *result) {
  for (int i = 0; i < 4; i++) {
    result->bits[i] = value->bits[i];
  }
}

int s21_bit(const s21_decimal value, const int num) {
  return (value.bits[num / 32] >> (num % 32)) & 1u;
}

int s21_mem_to_dec(unsigned *src, int scale, int sign, int size,
                   s21_decimal *result) {
  s21_clear_bits(result->bits, 4);
  int err = 0;
  int last_digit = 0;
  while ((!s21_arr_is_zero(&src[3], size - 3) || scale > 28) && scale > 0) {
    last_digit = s21_mod_int(src, 10, size);
    s21_div_int(src, src, 10, size);
    scale--;
  }
  if (last_digit == 5 && (s21_mod_int(src, 10, size) % 2 == 1)) {
    s21_add_int(src, src, 1, size);
  } else if (last_digit > 5) {
    s21_add_int(src, src, 1, size);
  }
  if (!s21_arr_is_zero(&src[3], size - 3)) {
    err = sign + 1;
  } else {
    memcpy(result->bits, src, 3 * sizeof(unsigned));
    result->bits[3] = 0u;
    s21_set_sign(result, sign);
    s21_set_scale(result, scale);
  }
  return err;
}

int s21_arr_is_zero(unsigned *arr, int size) {
  int flag = 1;
  for (int i = 0; i < size; i++) {
    if (arr[i] != 0) {
      flag &= 0;
    }
  }
  return flag;
}

int s21_sub_mem(const unsigned *value1, const unsigned *value2,
                unsigned *result, int size) {
  unsigned value2_copy[size];
  memcpy(value2_copy, value2, size * sizeof(unsigned));
  s21_negate_mem(value2, value2_copy, size);
  s21_mem_add(value1, value2_copy, result, size);
  return 0;
}