#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

#define PARSE_ARGS(fmt, args, OUTPUT_CODE) \
  for (const char *p = fmt; *p != '\0'; p++){ \
    if (*p == '%' && *(p + 1) != '\0'){ \
      p++; \
      switch (*p) { \
        case 'd': { \
          int value = va_arg(args, int); \
          if (value == 0){OUTPUT_CODE('0'); i++; break;} \
          if (value < 0) {OUTPUT_CODE('-'); i++; value = 0 - value;} \
          int div = 1; while (value / div >= 10){div *= 10;} \
          while (div > 0) { int digit = value / div; OUTPUT_CODE(digit + '0'); i++; value %= div; div /= 10;} \
          break; \
        } \
        case 's': { \
          char *s = va_arg(args, char *); \
          while (*s) { OUTPUT_CODE(*s); s++; i++;} \
          break; \
        } \
        default: break; \
      } \
    } \
    else {OUTPUT_CODE(*p);i++;} \
  }

int printf(const char *fmt, ...) {
  int i = 0; va_list args; va_start(args, fmt);
  PARSE_ARGS(fmt, args, putch);
  va_end(args);
  return i;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  int i = 0;
  PARSE_ARGS(fmt, ap, *out++ =);
  *out = '\0';
  return i;
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
    char *start = out;
    while (n-- && *fmt != '\0') {
        if (*fmt == '%') {
            fmt++;
            if (*fmt == 's') {
                char *tmp_s = va_arg(ap, char*);
                while (*tmp_s != '\0') {
                    *out++ = *tmp_s++;
                }
            }
            else if (*fmt == 'd') {
                int tmp_int = va_arg(ap, int);
                if (tmp_int < 0) {
                    *out++ = '-';
                    tmp_int = -1 * tmp_int;
                }
                int number = tmp_int;
                int len  = 0;
                do {
                    number /= 10;
                    len++;
                } while (number);
                out = out + len - 1;
                int tmp_len = len;
                while (tmp_len--) {
                    int tmp = tmp_int % 10;
                    *out-- = tmp + 48;
                    tmp_int /= 10;
                }
                out += (len+1);
            }
            else if (*fmt == '%') {
                *out++ = '%';
            }
            else if (*fmt == 'c') {
                char tmp_char = va_arg(ap, int);
                *out++ = tmp_char;
            }
            else {
                return -1;
            }
        }
        else {
            *out++ = *fmt;
        }
        fmt++;
    }
    *out = '\0';
    return out - start;
}

int sprintf(char *out, const char *fmt, ...) {
  int i = 0; va_list args; va_start(args, fmt);
  i= vsprintf(out, fmt, args); 
  va_end(args);
  return i;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

char *intToStr(int num, char* str) {
    int i = 0;
    int isNegative = 0;
    // 处理负数
    if (num < 0) {
        isNegative = 1;
        num = -num;
    }
    // 提取数字并转换为字符
    while (num != 0) {
        str[i++] = (num % 10) + '0';
        num = num / 10;
    }
    // 如果是负数，添加负号
    if (isNegative) {
        str[i++] = '-';
    }
    // 添加字符串结束符
    str[i] = '\0';
    // 反转字符串
    int len = strlen(str);
    for (int j = 0; j < len / 2; j++) {
        char temp = str[j];
        str[j] = str[len - j - 1];
        str[len - j - 1] = temp;
    }
  return str;
}

#endif
