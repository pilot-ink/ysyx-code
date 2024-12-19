#include <dlfcn.h>

#include "common.h"
#include "difftest.h"
#include "reg.h"
#include "paddr.h"

extern CPU_state cpu;
void (*ref_difftest_memcpy)(paddr_t addr, void *buf, size_t n, bool direction) = NULL;
void (*ref_difftest_regcpy)(void *dut, bool direction) = NULL;
void (*ref_difftest_exec)(uint64_t n) = NULL;
void (*ref_difftest_raise_intr)(uint64_t NO) = NULL;


void init_difftest(char *ref_so_file, long img_size, int port) {
  assert(ref_so_file != NULL);

  void *handle;
  handle = dlopen(ref_so_file, RTLD_LAZY);
  assert(handle);

  ref_difftest_memcpy = (void (*)(paddr_t addr, void *buf, size_t n, bool direction))dlsym(handle, "difftest_memcpy");
  assert(ref_difftest_memcpy);

  ref_difftest_regcpy = (void (*)(void *dut, bool direction))dlsym(handle, "difftest_regcpy");
  assert(ref_difftest_regcpy);

  ref_difftest_exec = (void (*)(uint64_t n))dlsym(handle, "difftest_exec");
  assert(ref_difftest_exec);

  ref_difftest_raise_intr = (void (*)(uint64_t NO))dlsym(handle, "difftest_raise_intr");
  assert(ref_difftest_raise_intr);

  void (*ref_difftest_init)(int) = (void (*)(int))dlsym(handle, "difftest_init");
  assert(ref_difftest_init);

  //Log("Differential testing: %s", ANSI_FMT("ON", ANSI_FG_GREEN));
  //Log("The result of every instruction will be compared with %s. "
   //   "This will help you a lot for debugging, but also significantly reduce the performance. "
   //   "If it is not necessary, you can turn it off in menuconfig.", ref_so_file);

  ref_difftest_init(port);
  printf("here\n");
  ref_difftest_memcpy(RESET_VECTOR, pmem, img_size, DIFFTEST_TO_REF);
  ref_difftest_regcpy(&cpu, DIFFTEST_TO_REF);
}

static void checkregs(CPU_state *ref, vaddr_t pc){
    if(!isa_difftest_checkregs(ref, pc)){
        flag = 1;
        isa_regs_display();
    }
}

void difftest_step(vaddr_t pc, vaddr_t npc){
    CPU_state ref = {};
    ref_difftest_exec(1);
    ref_difftest_regcpy(&ref, DIFFTEST_TO_DUT);
    checkregs(&ref, pc);
}