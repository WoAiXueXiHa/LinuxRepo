#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
  // 无效参数提示
  if(argc < 2){
    printf("当前指令错误！用法：\n");
    printf("选项: -add\r -sub\r -mul\r -div\n");
    printf("示例： ./calculate -add 10 20 -->计算10+20\n");
    return 1;
  }

  if(argc != 4){
    printf("参数个数错误！每个运算需要传入两个数字！\n");
    return 1;
  }

  int num1 = atoi(argv[2]);
  int num2 = atoi(argv[3]);

      if (strcmp(argv[1], "-add") == 0) {
        // 加法：num1 + num2
    } else if (strcmp(argv[1], "-sub") == 0) {
        printf("%d - %d = %d\n", num1, num2, num1 - num2);
    } else if (strcmp(argv[1], "-mul") == 0) {
        printf("%d × %d = %d\n", num1, num2, num1 * num2);
    } else if (strcmp(argv[1], "-div") == 0) {
        if (num2 == 0) {
            printf("错误：除数不能为0！\n");
            return 1;
        }
        printf("浮点数结果：%d ÷ %d = %.2f\n", num1, num2, (float)num1 / num2);
    } else {
        // 无效选项提示
        printf("无效选项「%s」！\n", argv[1]);
        printf("支持的选项：-add  -sub  -mul  -div\n");
        return 1;
    }

    return 0;

}
