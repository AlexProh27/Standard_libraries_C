#include "./s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int err = 0;
  int sign;
  int scale = s21_get_scale(value_1) > s21_get_scale(value_2)
                  ? s21_get_scale(value_1)
                  : s21_get_scale(value_2);
  unsigned buff1[6];
  unsigned buff2[6];
  unsigned buff_result[6];
  s21_clear_bits(buff_result, 6);
  s21_to_scale_mem(value_1, buff1, scale, 6);
  s21_to_scale_mem(value_2, buff2, scale, 6);

  if (s21_get_sign(value_1) == s21_get_sign(value_2)) {
    sign = s21_get_sign(value_1);
  } else {
    if (s21_is_greater_mem(buff1, buff2, 6)) {
      sign = s21_get_sign(value_1);
      s21_negate_mem(buff2, buff2, 6);
    } else {
      sign = s21_get_sign(value_2);
      s21_negate_mem(buff1, buff1, 6);
    }
  }
  s21_mem_add(buff1, buff2, buff_result, 6);
  err = s21_mem_to_dec(buff_result, scale, sign, 6, result);
  return err;
}
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_negate(value_2, &value_2);
  return s21_add(value_1, value_2, result);
}

int s21_div(s21_decimal value1, s21_decimal value2, s21_decimal *result) {
  int err = 0;
  s21_clear_bits(result->bits, 4);
  if (s21_dec_is_zero(value2)) {
    err = 3;
  } else {
    int sign = s21_get_sign(value1) != s21_get_sign(value2);
    unsigned buff1[9];
    unsigned buff2[9];
    unsigned buff_result[9];
    s21_clear_bits(buff_result, 9);
    s21_to_scale_mem(value1, buff1, 56, 9);
    s21_to_scale_mem(value2, buff2, 56, 9);
    int scale = -1;
    while (s21_is_greater_mem(buff1, buff2, 9)) {
      s21_mul_int(buff2, buff2, 10, 9);
      scale--;
    }
    while ((!s21_arr_is_zero(buff1, 9) && !s21_arr_is_zero(buff2, 9) &&
            scale < 30) ||
           scale < 0) {
      s21_mul_int(buff_result, buff_result, 10, 9);
      while ((!s21_is_greater_mem(buff2, buff1, 9))) {
        s21_sub_mem(buff1, buff2, buff1, 9);
        buff_result[0]++;
      }
      s21_div_int(buff2, buff2, 10, 9);
      scale++;
    }
    err = s21_mem_to_dec(buff_result, scale, sign, 9, result);
  }

  return err;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int err = 0;

  int sign = s21_get_sign(value_1) != s21_get_sign(value_2);
  int scale = s21_get_scale(value_1) + s21_get_scale(value_2);

  unsigned buff1[6];
  unsigned buff2[6];
  unsigned buff_result[6];

  s21_clear_bits(buff_result, 6);
  s21_to_scale_mem(value_1, buff1, s21_get_scale(value_1), 6);
  s21_to_scale_mem(value_2, buff2, s21_get_scale(value_2), 6);

  while (!s21_arr_is_zero(buff1, 6)) {
    if (buff1[0] % 2 == 1) {
      s21_mem_add(buff_result, buff2, buff_result, 6);
    }
    s21_div_int(buff1, buff1, 2, 6);
    s21_mul_int(buff2, buff2, 2, 6);
  }

  err = s21_mem_to_dec(buff_result, scale, sign, 6, result);
  return err;
}