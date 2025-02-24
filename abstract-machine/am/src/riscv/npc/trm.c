#include <am.h>
#include <klib-macros.h>

extern char _heap_start;
int main(const char *args);

extern char _pmem_start;
#define PMEM_SIZE (128 * 1024 * 1024)
#define PMEM_END  ((uintptr_t)&_pmem_start + PMEM_SIZE)
#define SERIAL_PORT 0xa00003f8

Area heap = RANGE(&_heap_start, PMEM_END);
static const char mainargs[MAINARGS_MAX_LEN] = MAINARGS_PLACEHOLDER; // defined in CFLAGS

void putch(char ch) {
  asm volatile("sb %0, 0(%1)" : : "r"(ch), "r"(SERIAL_PORT) : "memory");
}

void halt(int code) {
  __asm__("ebreak");
  while(1);
}

void _trm_init() {
  int ret = main(mainargs);
  halt(ret);
}
