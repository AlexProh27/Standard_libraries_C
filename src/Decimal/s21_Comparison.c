#include "s21_decimal.h"

int s21_is_less(s21_decimal value1, s21_decimal value2) {
  return s21_is_greater(value2, value1);
}

int s21_is_less_or_equal(s21_decimal value1, s21_decimal value2) {
  return s21_is_less(value1, value2) || s21_is_equal(value1, value2);
}

int s21_is_greater(s21_decimal value1, s21_decimal value2) {
  int result = 0;
  unsigned int buff1[6] = {0, 0, 0, 0, 0, 0};
  unsigned int buff2[6] = {0, 0, 0, 0, 0, 0};
  if ((result = s21_sign_compare(value1, value2)) != 0) {
    result = result == -1 ? 1 : 0;
  } else {
    for (int i = 0; i < 3; i++) {
      buff1[i] = value1.bits[i];
      buff2[i] = value2.bits[i];
    }
    s21_shift_scale(value1, value2, buff1, buff2, 6);
    if (s21_is_greater_mem(buff1, buff2, 6)) {
      result = 1;
    }
    if (s21_get_sign(value1) && !s21_is_equal(value1, value2)) {
      result = !result;
    }
  }
  return result && !s21_is_equal(value1, value2);
}

int s21_is_greater_or_equal(s21_decimal value1, s21_decimal value2) {
  return s21_is_greater(value1, value2) || s21_is_equal(value1, value2);
}

int s21_is_equal(s21_decimal value1, s21_decimal value2) {
  int result = 0;
  unsigned int buff1[6] = {0, 0, 0, 0, 0, 0};
  unsigned int buff2[6] = {0, 0, 0, 0, 0, 0};
  if (s21_dec_is_zero(value1) && s21_dec_is_zero(value2)) {
    result = 1;
  } else if ((result = s21_sign_compare(value1, value2)) != 0) {
    result = 0;
  } else {
    for (int i = 0; i < 3; i++) {
      buff1[i] = value1.bits[i];
      buff2[i] = value2.bits[i];
    }
    s21_shift_scale(value1, value2, buff1, buff2, 6);
    if (s21_is_equal_mem(buff1, buff2, 6)) {
      result = 1;
    }
  }
  return result;
}

int s21_is_not_equal(s21_decimal value1, s21_decimal value2) {
  return !s21_is_equal(value1, value2);
}