#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
  close(1);
  int fd1 = open("file1.txt",O_RDONLY | O_CREAT, 0666);
  int fd2 = open("file2.txt",O_RDONLY | O_CREAT, 0666);
  int fd3 = open("file3.txt",O_RDONLY | O_CREAT, 0666);
  int fd4 = open("file4.txt",O_RDONLY | O_CREAT, 0666);

  printf("fd:%d\n",fd1);
  printf("fd:%d\n",fd2);
  printf("fd:%d\n",fd3);
  printf("fd:%d\n",fd4);

  close(fd1);
  close(fd2);
  close(fd3);
  close(fd4);
  
  return 0;

}
