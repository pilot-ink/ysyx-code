#ifndef __DIFFTEST__
#define __DIFFTEST__

#include "common.h"


enum { DIFFTEST_TO_DUT, DIFFTEST_TO_REF };

void init_difftest(char *ref_so_file, long img_size, int port);
void difftest_step(vaddr_t pc, vaddr_t npc);

#define CONFIG_PC_RESET_OFFSET 0x0
#define PMEM_LEFT  ((paddr_t)CONFIG_MBASE)
#define PMEM_RIGHT ((paddr_t)CONFIG_MBASE + CONFIG_MSIZE - 1)
#define RESET_VECTOR (PMEM_LEFT + CONFIG_PC_RESET_OFFSET)



#endif