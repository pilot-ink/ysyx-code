#include "common.h"
#include <sys/time.h>
#include "paddr.h"
#include "trace.h"


static uint64_t get_time();
uint8_t* guest_to_host(paddr_t paddr) { return pmem + paddr - CONFIG_MBASE; }
paddr_t host_to_guest(uint8_t *haddr) { return haddr - pmem + CONFIG_MBASE; }

static uint64_t boot_time = 0;
static uint32_t rtc_port_base[2] = {};

extern "C" int pmem_read(int raddr) {
  // 总是读取地址为`raddr & ~0x3u`的4字节返回
    //printf("raddr:0x%08x\tinst:%08x\n",raddr, *((uint32_t *)guest_to_host(raddr)));
  push_mringbuf('r', raddr, *((uint32_t *)guest_to_host(raddr)));
  if(raddr == RTC_ADDR) 
  {
    uint64_t us = get_time();
    rtc_port_base[0] = (uint32_t)us;
    rtc_port_base[1] = us >> 32;
    return rtc_port_base[0]; 
  }
  else if(raddr == RTC_ADDR+4)
  {
    uint64_t us = get_time();
    rtc_port_base[0] = (uint32_t)us;
    rtc_port_base[1] = us >> 32;
    return rtc_port_base[1];
  }
  return *((uint32_t *)guest_to_host(raddr));
}
extern "C" void pmem_write(int waddr, int wdata, char wmask) {
  // 总是往地址为`waddr & ~0x3u`的4字节按写掩码`wmask`写入`wdata`
  // `wmask`中每比特表示`wdata`中1个字节的掩码,
  // 如`wmask = 0x3`代表只写入最低2个字节, 内存中的其它字节保持不变
  //printf("0x%08x\n",waddr);
  //printf("waddr:0x%08x\tdata:%08x\n",waddr,wdata);
  if (waddr == SERIAL_PORT) {
    char ch = wdata & 0xFF;
		putchar(ch);
		//serial[0] = ch;
    return ;
  }
  push_mringbuf('r', waddr, wdata);
  uint8_t *ptr = (uint8_t *)(guest_to_host(waddr));
    switch(wmask)
    {
        case 0x1: 
            *ptr = (uint8_t)(wdata & 0x000000FF);
            //printf("data:0x%08x\n",(wdata & 0xFF));
            break;
        case 0x2:
            *(uint16_t *)ptr = (uint16_t)(wdata & 0x0000FFFF);
            //printf("data:0x%08x\n",(uint16_t)(wdata | 0x00001111));
            break;
        default:
            *(uint32_t *)ptr = wdata;
    } 
}



void init_mem()
{
    pmem = (uint8_t *)malloc(CONFIG_MSIZE);
    assert(pmem);
}


static uint64_t get_time()
{
  struct timeval now;
  gettimeofday(&now, NULL);
  if(boot_time == 0) boot_time = now.tv_sec * 1000000 + now.tv_usec;  
  return (now.tv_sec * 1000000 + now.tv_usec - boot_time);
}