#include "mystrlen.h"

int mystrlen(const char* s){
  const char* end = s;
  while(*end != '\0') end++;
  return end - s;
}
