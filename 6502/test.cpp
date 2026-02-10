#include <stdio.h>
#include <stdlib.h>
using u32 = unsigned int;
using Byte = unsigned char;
using Word = unsigned short;

Word a=0X1234;
int main(){
    Word b= (a >> 8) & 0XFF;
    Word c= a & 0XFF;
    printf("%d",a);
    printf("\n%d",b);
    printf("\n%d",c);
    
    
    return 0;
}