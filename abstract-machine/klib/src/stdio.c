#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
char *intToStr(int num, char* str);
int printf(const char *fmt, ...) {
  panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  const char *p;
  char *ptr;
  char *ptr_out = out;
  char str[80];
  
  va_start(ap, fmt);
  
  for(p = fmt;*p;p++){
    if(*p != '%'){
      *++ptr_out = *p;
      continue;
    }
    switch(*++p){
      case 'd':
        for(ptr = intToStr(va_arg(ap, int),str);*ptr;ptr++)
          *++ptr_out = *ptr;
        break;
      case 's':
        for(ptr = va_arg(ap,char *);*ptr;ptr++)
          *++ptr_out = *ptr;
        break;
    }
  }
  *ptr_out = '\0';
  va_end(ap);
  return 0;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
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
