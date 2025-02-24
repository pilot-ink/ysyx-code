#include <am.h>
#include <nemu.h>
#include "string.h"

#define SYNC_ADDR (VGACTL_ADDR + 4)
#define N 32

void __am_gpu_init() {
  int i;
  int w = io_read(AM_GPU_CONFIG).width / N;  // TODO: get the correct width
  int h = io_read(AM_GPU_CONFIG).height / N;  // TODO: get the correct height
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < w * h; i ++) fb[i] = i;
  outl(SYNC_ADDR, 1);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  uint32_t space = inl(VGACTL_ADDR);
  uint32_t w = space >> 16;
  uint32_t h = space & 0x0ffff;
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = w, .height = h,
    .vmemsz = 0
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  // 坐标 区域
  int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
  if (!ctl->sync && (w == 0 || h == 0)) return;

  uint32_t *pix = ctl->pixels;
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  uint32_t with = inl(VGACTL_ADDR) >> 16;
  for (int i = y; i < y + h; i++) {
    uint32_t *line_start = fb + with * i + x;
    uint32_t *pix_line = pix + w * (i - y);
    memcpy(line_start, pix_line, w * sizeof(uint32_t));
    }
  // refresh
  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
