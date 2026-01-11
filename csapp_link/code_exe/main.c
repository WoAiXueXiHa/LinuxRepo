#include <stdio.h>

extern int sum(int* arr, int num);
int array[2] = { 1, 2};

int main(){
    int val = sum(array, 2);
    return 0;
}