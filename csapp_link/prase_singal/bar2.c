// bar2.c
__attribute__((weak)) int x;  // 明确声明为弱符号
void f(){x = 111;}