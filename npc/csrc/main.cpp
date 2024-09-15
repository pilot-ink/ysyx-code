#include <nvboard.h>
#include <Vtop.h>
 
static TOP_NAME dut;
 
void nvboard_bind_all_pins(Vtop* top);
 
int main() {
  nvboard_bind_all_pins(&dut);
  nvboard_init();
 
  while(1) {
    nvboard_update();
    dut.eval(); // 一定要加！否则就没有更新电路状态！一定要加！否则就没有更新电路状态！ 一定要加！否则就没有更新电路状态！
  }
}
