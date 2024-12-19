/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <isa.h>
#include <cpu/cpu.h>
#include <difftest-def.h>
#include <memory/paddr.h>

__EXPORT void difftest_memcpy(paddr_t addr, void *buf, size_t n, bool direction) {
  uint8_t *ptr = (uint8_t *)buf;
  if(direction == DIFFTEST_TO_DUT){ //to npc
    *ptr = paddr_read(addr,n);
    printf("asd");
  }
  else { //to nemu
    printf("addr:%08x\n",addr);
    for(int i = 0; i < n; i++)
    {
      paddr_write(addr+i, 1, (word_t)ptr[i]);
      printf("addr:%08x\tdata%08x\n",addr+i,ptr[i]);
    }
    
  }
}

__EXPORT void difftest_regcpy(void *dut, bool direction) {
  word_t *ptr = (word_t *)dut;
  if(direction == DIFFTEST_TO_DUT){ //to npc
    for(int i = 0; i < 32;i++)
      ptr[i] = cpu.gpr[i];
      ptr[32] = cpu.pc;
  }
  else { //to nemu
    for(int i = 0; i < 32;i++)
      cpu.gpr[i] = ptr[i];
      cpu.pc = ptr[32];
  }
  
}

__EXPORT void difftest_exec(uint64_t n) {
  cpu_exec(n);
}

__EXPORT void difftest_raise_intr(word_t NO) {
  assert(0);
}

__EXPORT void difftest_init(int port) {
  void init_mem();
  init_mem();
  /* Perform ISA dependent initialization. */
  init_isa();
}
