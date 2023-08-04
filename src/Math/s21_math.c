#include "s21_math.h"

// вычисляет абсолютное значение целого числа
int s21_abs(int x) {
  if (x < 0) x *= -1;
  return x;
}

long double s21_acos(double x) {
  if (s21_isinf(x)) return s21_inf;
  if (s21_isnan(x)) return s21_nan;
  return (x < -1 || x > 1) ? s21_nan : (s21_pi_2 - s21_asin(x));
}

long double s21_asin(double x) {
  if (s21_isinf(x)) return s21_inf;
  if (s21_isnan(x)) return s21_nan;
  return (x < -1 || x > 1) ? s21_nan : s21_atan(x / s21_sqrt(1 - x * x));
}

// возвращает ближайшее целое число, не меньшее заданного значения
long double s21_ceil(long double x) {
  if (s21_isinf(x)) return s21_inf;
  if (s21_isnan(x)) return s21_nan;
  long long int rez = (long long int)x;
  // целочисленному типу long long int, что позволяет получить целую часть числа
  // x
  if (x > rez) {
    return rez + 1.0L;  // В данном коде 1.0L обозначает число с
                        // плавающей точкой двойной точности
  } else {
    return rez;
  }
}

long double s21_atan(double x) {
  double ax = s21_fabs(x);  // arctan(-x) = -arctan(x)
  double a =
      (ax > 1.0) ? 1.0 / ax : ax;  // arctan(1/x) = pi/2 - arctan(x) if x > 0
  double k = 1;
  long double temp = 0, pred = 1;
  while (s21_fabs(pred - temp) > 1e-10) {
    pred = temp;
    temp += s21_pow(a, k) / k - s21_pow(a, k + 2) / (k + 2);
    k += 4;
  }
  temp = s21_fmod(temp, s21_pi);
  long double result = (ax > 1.0) ? (s21_pi_2 - temp) : temp;
  int sign = (x < 0) ? -1 : 1;
  return sign * result;
}

long double s21_cos(double x) {
  long double rez = 0;
  if (s21_isnan(x) || s21_isinf(x)) return s21_nan;
  if ((x >= -s21_pi && x <= s21_pi)) {
    rez = s21_sin((s21_pi / 2) - x);
    // позволяет вычислить значение синуса для аргумента
    // затем применить формулу cos(x) = sin(pi/2 - x)
  } else {
    x = s21_mod(x);
    // используется для приведения аргумента функции x к диапазону от 0 до 2π,
    // если значение x выходит за пределы этого диапазона
    rez = s21_cos(x);
  }
  return rez;
}

long double s21_exp(double x) {
  if (s21_isnan(x)) return s21_nan;
  if (s21_isinf(x)) return x < 0 ? 0 : s21_inf;
  int num = x < 0;
  if (num) x = -x;
  long double result = 1, temp = x;
  unsigned n = 1;
  while (s21_fabs(temp) >= s21_EPS && result != s21_inf) {
    result += temp;
    temp = (temp * x) / ++n;
    if (result > s21_DBL_MAX) {
      result = s21_inf;
    }
  }
  return num ? (1 / result) : result;
}

// вычисляет абсолютное значение числа с плавающей точкой
long double s21_fabs(double x) {
  if (x < 0) x *= -1.0L;
  return x;
}

// возвращает ближайшее целое число, не превышающее заданное значение
long double s21_floor(double x) {
  long long int rez = (long long int)x;
  // целочисленному типу long long int, что позволяет получить целую часть числа
  // x
  if (s21_isinf(x)) return s21_inf;
  if (s21_isnan(x)) return s21_nan;
  if (x < rez) {
    rez = (long double)rez - 1.0L;  // В данном коде 1.0L обозначает число с
                                    // плавающей точкой двойной точности
  } else {
    return rez;
  }
  return rez;
}

long double s21_fmod(double x, double y) {
  long double res;
  if (x == s21_nan || x == s21_inf || x == -s21_inf || x != x || y == 0)
    res = s21_nan;
  else if (y == s21_inf || y == -s21_inf)
    res = x;
  else {
    long long int quot = 0;
    quot = (x * 1e+16) / (y * 1e+16);
    res = (long double)x - quot * (long double)y;
  }
  return res;
}

long double s21_log(double x) {
  long double result = 0;

  if (x < 0 || x == -s21_inf || s21_isnan(x)) {
    result = s21_nan;
  } else if (x == 0) {
    result = -s21_inf;
  } else if (x == s21_inf) {
    result = s21_inf;
  } else if (x == 1) {
    result = 0;
  } else {
    double N = 0.0, P = x, A = 0;
    while (P >= S21_n) {
      P /= S21_n;
      N++;
    }
    N += (P / S21_n);
    P = x;
    int j = 0;
    do {
      double L, R;
      A = N;
      L = (P / (s21_exp(N - 1.0)));
      R = ((N - 1.0) * S21_n);
      N = ((L + R) / S21_n);
      j++;
    } while (N != A && j < s21_MAX);
    result = N;
  }

  return result;
}

// вспомогателльная функция( "сворачивает" значение x в пределах диапазона (-pi,
// pi])
long double s21_mod(double x) {
  if (x > s21_pi) {
    x -= 2 * s21_pi;
    // значение x уменьшается на 2πбчтобы x оказался в диапазоне от -π до π
  } else if (x < s21_pi) {
    x += 2 * s21_pi;
  }
  // значение x увеличивается на 2π, чтобы x оказался в том же диапазоне
  return x;
}

long double s21_pow(double x, double y) {
  long double result = 0;
  if (x < 0 && (int)y != y) {
    result = s21_nan;
  } else if (x == 1 || y == 0) {
    result = 1;
  } else if (y != y) {
    result = s21_nan;
  } else {
    if (y < 0) {
      result = 1.0 / s21_pow(x, (-1) * y);
    } else if ((int)y == y) {
      result = s21_pow_2(x, (int)y);
    } else {
      result = s21_exp(y * s21_log(x));
    }
  }
  return result;
}

long double s21_pow_2(double a, int b) {
  long double res = 1;
  for (int i = 0; i < b; i++) res = res * a;
  return res;
}

//	вычисляет синус
long double s21_sin(double x) {
  long double y = 0;
  long double rez = x;
  long double i = 1.0;
  int sing = 1;  // знак первого члена ряда
  if (s21_isnan(x) || s21_isinf(x)) return s21_nan;
  while (s21_fabs(rez) > s21_EPS) {
    if ((x >= -s21_pi && x <= s21_pi)) {  // интервал радиан
      y += sing * rez;
      sing *= -1;  // меняем знак при каждой итерации
      rez = rez * x * x / (2 * i * (2 * i + 1));  // ряд Тейлора
      i = i + 1;  // хранитромер текущего члена ряда
    } else {
      x = s21_mod(x);
      rez = x;
    }
  }
  return y;
}

// вычисляет квадратный корень
long double s21_sqrt(double x) {
  long double upper = 0;
  if (s21_isinf(x)) {
    return s21_inf;
  }
  if (x > 0) {
    long double max_value = s21_DBL_MAX - 1;
    // уменьшенное на единицу, чтобы избежать переполнения.
    upper = max_value / 2;
    long double lower = 0;  // в качестве начального значения для поиска корня
    long double otvet = upper * upper;
    while (s21_fabs(otvet - x) > s21_EPS) {
      //(бинарный поиск)
      if (otvet > x) {
        max_value = upper;
        upper = ((max_value - lower) * 0.5L) + lower;
        // В первом случае (if (otvet > x)), новое значение upper выбирается
        // так, чтобы интервал поиска сужался влево, потому что квадрат текущего
        // значения upper больше x, и корень должен находиться в интервале
        // меньшем, чем текущий интервал.
      } else if (otvet < x) {
        lower = upper;
        upper = ((max_value - lower) * 0.5L) + lower;
        // Во втором случае (else if (otvet < x)), новое значение upper
        // выбирается так, чтобы интервал поиска сужался вправо, потому что
        // квадрат текущего значения upper меньше x, и корень должен находиться
        // в интервале большем, чем текущий интервал.
      } else
        break;
      otvet = upper * upper;
    }
  } else if (x == 0) {
    upper = 0;
  } else {
    upper = s21_nan;
  }
  return upper;
}
// в обоих случаях используется один и тот же код для вычисления нового значения
// верхней границы интервала upper. Да так надо, в этом состоит принцип
// бинарного поиска

// поиск тангенса через отношения косинуса к синусу
long double s21_tan(double x) {
  long double rez = 0.;

  if (s21_isnan(x) || s21_isinf(x)) return s21_nan;
  if (x != 0) {
    rez = s21_sin(x) / s21_cos(x);
  }
  return rez;
}
