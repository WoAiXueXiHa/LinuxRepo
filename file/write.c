#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(){
  int fd = open("log.txt",O_CREAT | O_WRONLY, 0666);  // fd文件描述符
  if(fd < 0){
    perror("open err");
    return 1;
  }

  const char* msg = "hello file\n";
  char buffer[1024];
  while(1){
    ssize_t s = read(fd,buffer,strlen(msg));
    if(s > 0){
      printf("%s",buffer);
    }else break;
  }

  close(fd);

  return 0;
}
