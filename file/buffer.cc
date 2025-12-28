#include <cstdio>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
  close(1);
  int fd = open("log.txt",O_WRONLY | O_CREAT | O_TRUNC, 0666);
  fprintf(stdout,"hhhhhhhhhhhhhh\n");
  //fflush(stdout);
  close(fd);

  return 0;


}
