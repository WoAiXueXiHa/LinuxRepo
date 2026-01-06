#include <stdio.h>
#include <string.h>

int main(){
  FILE* fp = fopen("myfile","r");
  if(!fp){
    perror("fp err");
    return 1;
  }

  char buffer[1024];
  const char* msg = "hello file\n";
  while(1){
    ssize_t s = fread(buffer,1,strlen(msg),fp);
    if(s > 0){
      buffer[s] = 0;
      printf("%s",buffer);
    }
    if(feof(fp)){
      break;
    }
  }
  fclose(fp);
  return 0;

}
