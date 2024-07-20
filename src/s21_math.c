#include "s21_math.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define eps (1e-8)
#define circleRad (6.2831853071795865)
#define s21_nan (0.0/0.0L)
#define pos_inf (1.0/0.0L)
#define neg_inf (-1.0/0.0L)
#define pi (3.1415926535897932)
#define pi12 (pi / 12.F)
#define pi6 (pi / 6.F)
#define pi2 (pi / 2.F)
#define sqrt3 (1.732050807569)
#define e (2.7182818284590452353602874713527)
#define LDBL_MAX (1.79769313486231571e+308)
#define LDBL_MIN (2.22507385850720138e-308)

// вычисляет абсолютное значение целого числа
long int s21_abs(int x) {
    if (x < 0)
        x = -x;
    return x;
}

// вычисляет арксинус
long double s21_asin(double x) {
    long double summ;  // сумма ряда

    if (s21_isinf(x) || s21_isnan(x)) {
        summ = s21_nan;
    } else if (x < -1.F) {
        summ = s21_nan;
    } else if (x == -1.F) {
        summ = -pi2;
    } else if (x > 1.F) {
        summ = s21_nan;
    } else if (x == 1.F) {
        summ = pi2;
    } else {
        int sign = 0;
        if (x < 0) {
            sign = 1;
            x = -x;
        }

        summ = x;
        long double sn = x;       // n-й член ряда
        for (long double n = 1; s21_fabs(sn) > 1e-17; n += 1) {
            long double kn = ((2 * n - 1) * (2 * n - 1) * x * x) / (4 * n * n + 2 * n);
            sn *= kn;
            summ += sn;
        }
        summ = sign ? -summ : summ;
    }
    return summ;
}

// вычисляет арккосинус tmicaela
long double s21_acos(double x) {
    long double y;
    if (s21_isinf(x) || s21_isnan(x) || x > 1.F || x < -1.F) {
        y = s21_nan;
    } else if (x == 1.F) {
        y = 0;
    } else {
        y = s21_asin(x);
        y = pi2 - y;
    }
    return y;
}

// вычисляет арктангенс
long double s21_atan(double x) {
    long double result = 0;

    if (x == pos_inf) {
        result = pi2;
    } else if (x == neg_inf) {
        result = -pi2;
    } else if (s21_isnan(x)) {
        result = s21_nan;
    } else if (-1 < x && x < 1) {
        result = s21_asin((long double)x / s21_sqrt(1 + x * x));
    } else if (x == 1) {
        result = pi / 4.F;
    } else if (x == -1) {
        result = -pi / 4.F;
    } else if (x == 0) {
        result = 0;
    } else if (x > 1) {
        x = 1 / x;
        result = pi / 2.F - s21_asin((long double)x / s21_sqrt(1 + x * x));
    } else {
        // x < -1
        x = 1 / x;
        result = -pi / 2.F - s21_asin((long double)x / s21_sqrt(1 + x * x));
    }
    return result;
}

// возвращает ближайшее целое число, не меньшее заданного значения
long double s21_ceil(double x) {
    long double temp = x;
    if (!s21_isnan(x) && !s21_isinf(x)) {
        if (x >= LDBL_MAX - 1) {
            temp = LDBL_MAX;
        } else {
            temp = s21_fabs(s21_floor(x) - x) == 0 ? s21_floor(x) :
                                                        s21_floor(x) + 1;
        }
    }
    return temp;
}

// вычисляет косинус tmicaela
long double s21_cos(double x) {
    long double s;
    if (s21_isinf(x) || s21_isnan(x)) {
        s = s21_nan;
    } else {
        s = s21_sin(x + pi2);
    }
    return s;
}

// возвращает значение e, возведенное в заданную степень
long double s21_exp(double x) {
    long double s = 1, d = 1;
    long double sum_temp;
    if (x == 0.0) {
        s = 1.0;
    } else if (s21_isnan(x)) {
        s = s21_nan;
    } else if (x == pos_inf) {
        s = pos_inf;
    } else if (x == neg_inf) {
        s = 0;
    } else {
        for (double n = 1; s21_fabs(d) > (1e-16) ; n++) {
            d *= (x - (int)x) / n;
            s += d;
        }
        sum_temp = help_pow(e, (int)x);
        s *= sum_temp;
    }
    return s;
}

// вычисляет абсолютное значение числа с плавающей точкой
long double s21_fabs(double x) {
    long double temp = x;

    if (s21_isnan(x)) {
        temp = s21_nan;
    } else if (x == -neg_inf) {
        temp = pos_inf;
    } else if (temp < 0) {
        temp = -temp;
    }
    return temp;
}

// возвращает ближайшее целое число, не превышающее заданное значение
long double s21_floor(double x) {
    long double result = x;
    if (x != pos_inf && x != neg_inf && !s21_isnan(x)) {
        if (x <= -LDBL_MAX + 1) {
            result = -LDBL_MAX;
        } else {
            int sign = x < 0 ? 1 : 0;
            x = sign ? -x : x;

            char strDbl[350] = {0};
            snprintf(strDbl, sizeof(strDbl), "%.30lf", x);

            // получим целую и дробную часть
            char *ptr = strchr(strDbl, '.');
            double fractPart = atof(ptr);
            *ptr = '\0';
            ptr = NULL;
            double intPart = atof(strDbl);

            if (fractPart == 0) {
                // нулевая дробная часть
                result = sign ? -intPart : intPart;
            } else if (sign) {
                result = -intPart - 1;
            } else {
                result = intPart;
            }
        }
    }
    return result;
}

// остаток операции деления с плавающей точкой
long double s21_fmod(double x, double y) {
    long double temp;
    if (s21_isinf(y) && !s21_isinf(x) && !s21_isnan(x) && !s21_isnan(y)) {
        temp = x;
    } else if (y != 0 && !s21_isinf(x) && !s21_isnan(x) && !s21_isnan(y)) {
        if (s21_fabs(x) < s21_fabs(y)) {
            temp = x;
        } else {
            // сначала поделим
            long double division = (long double)x / y;

            // перепишем результат деления в строку
            char strDbl[350] = {0};
            snprintf(strDbl, sizeof(strDbl), "%.30Lf", division);

            // получим целую часть division
            char *ptr = strchr(strDbl, '.');
            *ptr = 0;
            ptr = NULL;
            long double intPart = atof(strDbl);

            // получаем результат
            temp = x - intPart * y;
        }
    } else {
        temp = s21_nan;
    }
    return temp;
}

// вычисляет натуральный логарифм
long double s21_log(double x) {
    long double s;
    if (s21_isnan(x) || s21_isinf(x) || x <= 0) {
        if (x == pos_inf) {
            s = pos_inf;
        } else if (x == 0) {
            s = neg_inf;
        } else {
            s = s21_nan;
        }
    } else {
        int n = 0;
        while (x > 1) {
            x = x / e;
            n++;
        }
        s = n + s21_log_min(x);
    }
    return s;
}

// возводит число в заданную степень
long double s21_pow(double base, double exp) {
    long double temp = 1;
    if (base == 1.0 || exp == 0) {
        temp = 1;
    } else if (base == 0) {
        if (exp < 0) {
            temp = pos_inf;
        } else {
            temp = 0;
        }
    } else if (s21_isnan(base) || s21_isnan(exp)) {
        temp = s21_nan;
    } else if (s21_isinf(base) || s21_isinf(exp)) {
        if (exp == neg_inf) {
            if (base == -1) {
                temp = 1;
            } else if (base > -1 && base < 1) {
                temp = pos_inf;
            } else {
                temp = 0;
            }
        } else {
            if (base == -1) {
                temp = 1;
            } else if (base > -1 && base < 1) {
                temp = 0;
            } else {
                temp = pos_inf;
            }
        }
    } else if (s21_fabs(s21_fmod(exp, 1.0)) < eps) {
        if (exp >= 0) {
            for (int i = 0; i < (int)exp; ++i)
                temp *= base;
        } else {
            exp = -exp;
            for (int i = 0; i < (int)exp; ++i)
                temp /= base;
        }
    } else {
        temp = s21_exp(exp * s21_log(s21_fabs(base)));
        if (base < 0 && s21_fmod(exp, (int)exp) == 0 && (int)exp % 2 != 0) {
            temp *= -1;
        } else if (base < 0 && s21_fmod(exp, (int)exp) != 0) {
            temp = s21_nan;
        }
    }
    return temp;
}

// вычисляет синус
long double s21_sin(double x) {
    long double s;
    if (x == neg_inf || x == pos_inf || s21_isnan(x)) {
        s = s21_nan;
    } else {
        x = s21_fmod(x, circleRad);
        s = x;
        long double d = x;
        for (int n = 3; s21_fabs(d) > eps; n += 2) {
            d *= -x * x / n / (n - 1);
            s += d;
        }
    }
    return s;
}

// вычисляет квадратный корень
long double s21_sqrt(double x) {
    long double s = 0;
    if (s21_isnan(x) || x == neg_inf) {
        s = s21_nan;
    } else if (x == pos_inf) {
        s = pos_inf;
    } else {
        if (x > 0) {
            s = x / 2;
            long double d;
            do {
                d = s;
                s = (d + (x / d)) / 2;
            } while ((d - s) != 0);
        } else if (x < 0) {
            s = s21_nan;
        }
    }
    return s;
}

// вычисляет тангенс
long double s21_tan(double x) {
    return s21_sin(x) / s21_cos(x);
}

// Доп
int s21_isnan(double x) {
    int temp = 0;
    if (x != x) {
        temp = 1;
    }
    return temp;
}

int s21_isinf(double x) {
    int ret = 0;
    if (x == pos_inf || x == neg_inf) {
        ret = 1;
    }
    return ret;
}

long double s21_log_min(double x) {
    long double s;
    long double sn = x - 1.0;
    s = sn;
    for (int i = 0; i == 0 || s21_fabs(sn - s) > eps; i++) {
        sn = s;
        s = sn + 2 * (x - s21_exp(sn)) / (x + s21_exp(sn));
    }
    return s;
}
long double help_pow(double base, int exp) {
    long double res = 1;
    int count = s21_abs(exp);
    for (int i = 1; i <= count; i++) {
        res *= base;
    }
    if (exp < 0) res = 1.0 / res;
    return res;
}
