#include <stdio.h>
#include <stdlib.h>

int main(){
  char* path = getenv("PATH");
  printf("PATH:%s\n",path);

  putenv("MYENV=hhhh"); // 设置环境变量
  
  return 0;
}
