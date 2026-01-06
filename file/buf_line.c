#include <stdio.h>
#include <unistd.h>

int main(){
  // 1. 无\n 数据保留在缓冲区 不输出
  printf("无换行的输出");
  sleep(3);

  // 2. 手动fflush刷新缓冲区 屏幕立即输出
  fflush(stdout);
  sleep(3);

  // 3. 加\n 自动刷新缓冲区
  printf("\n有换行的输出");
  sleep(3);

  return 0;
}
