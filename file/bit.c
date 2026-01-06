#include <stdio.h>

#define ONE   0001    // 0000 0001
#define TWO   0002    // 0000 0010
#define THREE 0004    // 0000 0100

void func(int flags){
  if(flags & ONE) printf("ONE\n");
  if(flags & TWO) printf("TWO\n");
  if(flags & THREE) printf("THREE\n");
  printf("-----------\n");
}

int main(){
  func(ONE);
  func(ONE | TWO);
  func(ONE | TWO | THREE);

  return 0;
}
