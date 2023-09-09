#include "./s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  unsigned point = 0;
  if (s21_get_sign(value) == 0) {
    s21_truncate(value, result);
  } else {
    memcpy(result->bits, value.bits, sizeof(unsigned) * 4);
    int ratio = s21_get_scale(*result);
    for (int i = 0; i < ratio; i++) {
      point |= s21_mod_int(result->bits, 10, 3) > 0;
      s21_div_int(result->bits, result->bits, 10, 3);
    }
    if (point) {
      s21_add_int(result->bits, result->bits, 1, 3);
    }
    s21_set_scale(result, 0);
  }
  return 0;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int er = 0;
  unsigned int sign = s21_get_sign(value);
  int ratio = s21_get_scale(value);
  memcpy(result->bits, value.bits, sizeof(unsigned) * 4);
  for (; ratio > 0; ratio--) {
    s21_div_int(result->bits, result->bits, 10, 3);
  }
  s21_set_scale(result, 0);
  s21_set_sign(result, sign);
  return er;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  *result = value;
  ((value.bits[3] >> 31 & 1) == 1) ? s21_set_sign(result, 0)
                                   : s21_set_sign(result, 1);
  return 0;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  unsigned last_digit = 0;
  memcpy(result->bits, value.bits, sizeof(unsigned) * 4);
  int scale = s21_get_scale(*result);
  for (int i = 0; i < scale; i++) {
    last_digit = s21_mod_int(result->bits, 10, 3);
    s21_div_int(result->bits, result->bits, 10, 3);
  }
  if (last_digit >= 5) {
    s21_add_int(result->bits, result->bits, 1, 3);
  }
  s21_set_scale(result, 0);
  return 0;
}