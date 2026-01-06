#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

int main(){
 // close(1);
 // int fd = open("mychdir",O_WRONLY | O_CREAT,00666);
 // if(fd < 0){
 //   perror("open err");
 //   return 1;
 // }
 // printf("fd:%d\n",fd);
 // fflush(stdout);

 // close(fd);
 //
 
  int fd = open("mydup2",O_CREAT | O_RDWR | O_APPEND,0666);
  if(fd < 0){
    perror("open err");
    return 1;
  }
  const char* msg = "hello\n";
  dup2(fd,1);
  int cnt = 3;
  while(cnt--){
    write(1,msg,strlen(msg));
  }

  close(fd);
  return 0;
}
