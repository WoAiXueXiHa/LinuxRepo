#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(){
  const char* msg1 = "hello printf\n";
  const char* msg2 = "hello fwrite\n";
  const char* msg3 = "hell write\n";

  printf("%s",msg1);// 用户态缓冲
  fwrite(msg2,strlen(msg2),1,stdout);// 用户态缓冲
  write(1,msg3,strlen(msg3)); // 内核缓冲

  fork();
  return 0;
}
