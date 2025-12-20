#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]){
    // 第一步：检查参数个数，至少需要1个选项参数（argv[1]）
    if (argc < 2) { // 仅输入程序名，未传任何选项
        printf("错误：未传入选项！\n");
        printf("有效选项：-opt1 或 -opt2\n");
        printf("使用示例：./a.out -opt1\n");
        return 1; // 非0返回值表示程序异常退出
    }

    // 第二步：判断argv[1]（唯一的选项参数位置）
    if (strcmp(argv[1], "-opt1") == 0) {
        printf("程序选项1\n");
    } else if (strcmp(argv[1], "-opt2") == 0) {
        printf("程序选项2\n");
    } else {
        printf("错误：无效选项「%s」！\n", argv[1]);
        printf("有效选项：-opt1 或 -opt2\n");
        return 1;
    }

    return 0;
}
