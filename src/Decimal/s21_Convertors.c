#include "./s21_decimal.h"

/// Из float

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int result = 0;
  for (int i = 0; i < 4; i++) {
    dst->bits[i] = 0;
  }
  if ((src != src) | (fabsf(src) > 7.922816e28) | (fabsf(src) == INFINITY) |
      (fabsf(src) < 1e-28)) {
    if (fabsf(src) != 0) {
      result = 1;
    }
  } else {
    char flts[64];
    snprintf(flts, 64, "%.6e", src);
    int sign_int = (src > 0 ? 0 : 1);
    src = fabsf(src);
    char numb[64];
    int j = 0;
    for (unsigned long int i = 0; i < strlen(flts); i++) {
      if (flts[i] == 'e') {
        numb[j] = '\0';
        break;
      }
      if (flts[i] != 46) {
        numb[j] = flts[i];
        j++;
      }
    }
    int _exp10 = 0;
    int _mantissa = 0;
    int tmp = 0;
    sscanf(numb, "%d", &_mantissa);
    sscanf(flts, "%d.%de%d", &tmp, &tmp, &_exp10);
    dst->bits[0] = abs(_mantissa);
    _exp10 = 6 - _exp10;
    while (_exp10 < 0) {
      s21_mod_int(dst->bits, 10, 3);
      s21_mul_int(dst->bits, dst->bits, 10, 3);
      _exp10++;
    }
    if (sign_int) {
      dst->bits[3] = 0x80000000;
    }
    s21_set_scale(dst, _exp10);
  }
  return result;
}
///// В float
int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int er = 0;
  int scale = s21_get_scale(src);
  if (dst == NULL || scale > 28) {
    er = 1;
  } else {
    double result = 0.0;
    for (int i = 0; i < 96; i++) {
      if (s21_bit(src, i) != 0) {
        result += pow(2, i);
      }
    }
    while (scale != 0) {
      result /= 10.0;
      scale--;
    }
    *dst = 0;
    *dst = (float)result;
    int sign = s21_get_sign(src);
    if (sign == 1) {
      *dst *= -1;
    }
  }
  return er;
}

/// В int
int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int er = 0;
  int scale = s21_get_scale(src);
  if (dst == NULL || scale > 28) {
    er = 1;
  } else {
    int sign = s21_get_sign(src);
    s21_decimal copy;
    s21_copy_dec_to_dec(&src, &copy);
    while (scale != 0) {
      s21_div_int(src.bits, copy.bits, 10, 3);
      s21_copy_dec_to_dec(&copy, &src);
      scale--;
    }
    if (src.bits[1] != 0 || src.bits[2] != 0 || (src.bits[0] >> 31 & 1) != 0) {
      er = 1;
    } else {
      *dst = 0;
      *dst = src.bits[0];
      if (sign == 1) {
        *dst *= -1;
      }
    }
  }
  return er;
}
// Из int
int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int er = 0;
  if (dst == NULL) {
    er = 1;
  } else {
    s21_clear_bits(dst->bits, 4);
    if (src < 0) {
      s21_set_sign(dst, 1);
      src = -src;
    }
    dst->bits[0] = src;
  }
  return er;
}
