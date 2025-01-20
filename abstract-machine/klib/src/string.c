#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  size_t i = 0;
  for(;s[i] != '\0'; i++) ;
  return i;
}

char *strcpy(char *dst, const char *src) {
  int i;
  for(i = 0; src[i] != '\0'; i++){
    dst[i] = src[i];
  }
  dst[i] = src[i];
  return dst;
}

char *strncpy(char *dst, const char *src, size_t n) {
  size_t i;
  for(i = 0; (i < n) && (src[i] != '\0');i++)
    dst[i] = src[i];
  for(;i < n;i++)
    dst[i] = '\0';
  return dst;
}

char *strcat(char *dst, const char *src) {
  int i = 0;
  int j;
  while (dst[i] != '\0')
  {
    i++;
  }
  for(j = 0;src[j] != '\0';j++)
    dst[i+j] = src[j];
  dst[i+j] = '\0';
  return dst;
}

int strcmp(const char *s1, const char *s2) {
  int i = strlen(s1)>=strlen(s2)?strlen(s1):strlen(s2);
  int j;
  for(j = 0;j < i;j++){
    if((s1[j] == '\0') || (s2[j] == '\0')) break;
    else if(s1[j] == s2[j]) continue;
    else if(s1[j] > s2[j])  return 1;
    else return -1;
  }
  return 0;
}

int strncmp(const char *s1, const char *s2, size_t n) {
  int j;
  for(j = 0;j < n;j++){
    if((s1[j] == '\0') || (s2[j] == '\0')) break;
    else if(s1[j] == s2[j]) continue;
    else if(s1[j] > s2[j])  return 1;
    else return -1;
  }
  return 0;
}

void *memset(void *s, int c, size_t n) {
  char *ptr = (char *)s;
  for(int i = 0;i < n;i++)
    ptr[i] = c;
  return s;
}

void *memmove(void *dst, const void *src, size_t n) {
  char *d = (char *)dst;
    const char *s = (const char *)src;

    if (s >= d) {
        while (n) {
            *d++ = *s++;
            --n;
        }
    } else {
        while (n) {
            --n;
            d[n] = s[n];
        }
    }

    return dst;
}

void *memcpy(void *out, const void *in, size_t n) {
  char *ptr_out = (char *)out;
  char *ptr_in = (char *)in;
  for(int i = 0;i < n;i++)
    ptr_out[i] = ptr_in[i];
  return out;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  int i = strlen((char *)s1)>=strlen((char *)s2)?strlen((char *)s1):strlen((char *)s2);
  int j;
  char *ptr_s1 = (char *)s1;
  char *ptr_s2 = (char *)s2;
  for(j = 0;j < i;j++){
    if((ptr_s1[j] == '\0') || (ptr_s2[j] == '\0')) break;
    else if(ptr_s1[j] == ptr_s2[j]) continue;
    else if(ptr_s1[j] > ptr_s2[j])  return 1;
    else return -1;
  }
  return 0;
}

#endif
