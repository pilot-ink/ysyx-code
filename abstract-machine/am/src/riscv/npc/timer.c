#include <am.h>
#define RTC_ADDR 0xa0000048

void __am_timer_init() {
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  uint32_t time_h, time_l;
  asm volatile("lw %0, 4(%1)" : "=r"(time_h) : "r"(RTC_ADDR) : "memory");
  asm volatile("lw %0, 0(%1)" : "=r"(time_l) : "r"(RTC_ADDR) : "memory");
  uptime->us = (uint64_t)time_l + ((uint64_t)time_h << 32);
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
