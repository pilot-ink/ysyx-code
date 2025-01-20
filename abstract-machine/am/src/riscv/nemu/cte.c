#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;

Context* __am_irq_handle(Context *c) {
  if (user_handler) {
    Event ev = {0};
    switch (c->mcause) {
      case 17 : ev.event = EVENT_YIELD; break;
      default: ev.event = EVENT_ERROR; break;
    }

    c = user_handler(ev, c);
    assert(c != NULL);
  }

  //c->mepc += 4;
  return c;
}

extern void __am_asm_trap(void);

//CTE相关的进行初始化操作
//还接受一个来自操作系统的事件处理回调函数的指针, 当发生事件时, 
//CTE将会把事件和相关的上下文作为参数, 来调用这个回调函数, 交由操作系统进行后续处理.
/*
* 将异常入口地址设置到mtvec寄存器中
* 注册一个事件处理回调函数
*/
bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize exception entry
  /*
  * csrw 为指令  将异常入口装入mtvec
  */
  asm volatile("csrw mtvec, %0" : : "r"(__am_asm_trap));

  // register event handler
  user_handler = handler;

  return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
  Context *cp = (Context *)(kstack.end - sizeof(Context));
  cp->mepc = (uintptr_t)entry - 4;
  cp->gpr[10] = (uintptr_t)(arg);
  return cp;
}
//自陷操作
void yield() {
#ifdef __riscv_e
  asm volatile("li a5, -1; ecall");
#else
  asm volatile("li a7, -1; ecall");
#endif
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
