#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int global_val = 10;
int g_val;
int main(int argc, char* argv[], char* env[]){
  const char* str = "hello";
  printf("code addr: %p\n",main);
  printf("init global_val addr: %p\n",&global_val);
  printf("uninit g_val addr: %p\n",&g_val);
   
  static int test_val = 1;
  char* heap_mem1 = (char*)malloc(10);  
  char* heap_mem2 = (char*)malloc(10);
  char* heap_mem3 = (char*)malloc(10);
  printf("haep addr: %p\n",heap_mem1);
  printf("haep addr: %p\n",heap_mem2);
  printf("haep addr: %p\n",heap_mem3);

  printf("test_val static addr: %p\n",&test_val);

  printf("stack addr: %p\n",&heap_mem1);
  printf("stack addr: %p\n",&heap_mem2);
  printf("stack addr: %p\n",&heap_mem3);

  printf("read only string add: %p\n",str);
  for(int i = 0; i < argc; i++){
    printf("argv[%d]: %p\n",i,argv[i]);
  }
  for(int i = 0; env[i]; i++){
    printf("env[%d]: %p\n",i, env[i]);
  }

  return 0;
}
