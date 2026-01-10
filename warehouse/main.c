#include "mystdio.h"
#include "mystrlen.h"
#include <stdio.h>

int main(){
  const char* msg = "abc";
  printf("%s:%d\n",msg,mystrlen(msg));

  mFILE* fp = mfopen("log.txt","a");
  if(!fp) return 1;

  mfwrite(msg,mystrlen(msg),fp);

  mfclose(fp);

  return 0;
}
