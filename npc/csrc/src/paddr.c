#include "common.h"



uint8_t* guest_to_host(paddr_t paddr) { return pmem + paddr - CONFIG_MBASE; }
paddr_t host_to_guest(uint8_t *haddr) { return haddr - pmem + CONFIG_MBASE; }


extern "C" int pmem_read(int raddr) {
  // 总是读取地址为`raddr & ~0x3u`的4字节返回
    printf("raddr:0x%08x\tinst:%08x\n",raddr, *((uint32_t *)guest_to_host(raddr)));
  return *((uint32_t *)guest_to_host(raddr));
}
extern "C" void pmem_write(int waddr, int wdata, char wmask) {
  // 总是往地址为`waddr & ~0x3u`的4字节按写掩码`wmask`写入`wdata`
  // `wmask`中每比特表示`wdata`中1个字节的掩码,
  // 如`wmask = 0x3`代表只写入最低2个字节, 内存中的其它字节保持不变
  //printf("0x%08x\n",waddr);
  printf("waddr:0x%08x\tdata:%08x\n",waddr,wdata);
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