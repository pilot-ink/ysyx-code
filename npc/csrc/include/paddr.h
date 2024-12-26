#ifndef __PADDR__
#define __PADDR__



uint8_t* guest_to_host(paddr_t paddr);
paddr_t host_to_guest(uint8_t *haddr);
void init_mem();



#endif