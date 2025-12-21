#include <stdio.h>
#include <stdlib.h>

int main(){
  char* env = getenv("MYENV");

  if(env == NULL){
    printf("未找到\n");
  }else{
    printf("成功读取MYENV：%s\n",env);
  }

  return 0;
}
