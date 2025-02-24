#ifndef __PADDR__
#define __PADDR__



uint8_t* guest_to_host(paddr_t paddr);
paddr_t host_to_guest(uint8_t *haddr);
void init_mem();

#define RTC_ADDR 0xa0000048
#define SERIAL_PORT 0xa00003f8

#endif