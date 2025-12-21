#include <stdio.h>

extern char** environ;    // 不在头文件中，需要提前声明
int main(){
  for(int i = 0; environ[i]; i++){
    printf("%s\n",environ[i]);
  }
  return 0;
}
