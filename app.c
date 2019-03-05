/**
 * GitHub：https://github.com/1459177541/Polynomial-operation
 **/
#include<stdio.h>
typedef struct polynomial
{
    double num;
    double power;
}polynomial, *pPolynomial;
typedef struct list
{
    void *data;
    struct list *next;
}list, *pList;

//加法
pPolynomial addition(){

    return NULL;
}

//减法
pPolynomial subtraction(){

    return NULL;
}

//乘法
pPolynomial multiplication(){

    return NULL;
}

//输出
void printPolynomial(pPolynomial polynomial){
    
}

void menu(){
    printf(" ----菜单----\n");
    printf("1.多项式相加\n");
    printf("2.多项式相减\n");
    printf("3.多项式相乘\n");
    printf("0.退出\n");
    printf("请输入您的选择:");
    char in = getchar();
    while('\n'!=getchar()){}    
    pPolynomial result;
    switch (in)
    {
        case '1':
            result = addition();
            break;
        case '2':
            result = subtraction();
            break;
        case '3':
            result = multiplication();
            break;
        case '0':
            return;
        default:
            printf("输入错误，请选择正确选项！\n");
            menu();
            return;
    }
    printPolynomial(result);
    printf("按回车键返回菜单\n");
    while('\n'!=getchar()){}    
    menu();
}

int main(){
    menu();
    printf("感谢使用!\n");
    return 0;
}