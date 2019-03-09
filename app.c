/**
 * GitHub：https://github.com/1459177541/Polynomial-operation
 **/
#include<stdio.h>
#include<malloc.h>
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

//输入
char* input(){
    int size = 32;
    int i = 0;
    char *input = (char*)malloc(sizeof(char)*size);
    char inChar;
    while('\n' != (inChar = getchar())){
        //扩容
        if (i >= size) {
            int newSize = size * 2;
            char *newInput =  (char*)malloc(sizeof(char)*newSize);
            for(int j = 0; j < i; j++)
            {
                newInput[j] = input[i];
            }
            free(input);
            input = newInput;
            size = newSize;
        }
        input[i] = inChar;
        i++;
    }
    input[i] = '\0';
    return input;
}

//解析一个
pPolynomial analysisTerm(char *in, int start, int end){
    pPolynomial d = (pPolynomial)malloc(sizeof(polynomial));
    int num = 0;
    double decimal = 0;
    double decimalMultiples = 1;
    int flag = 0;
    for(int i = start; i < end; i++)
    {
        //切换到x的指数
        if ('x'==in[i]) {
            d->num = num+decimal;
            num = 0;
            decimal = 0;
            decimalMultiples = 1;
            flag = 0;
            continue;
        }
        //非法字符
        if (('0'>in[i] || '9'<in[i]) && '.'!=in[i]) {
            continue;
        }
        //切换到小数
        if ('.' == in[i]) {
            flag = 1;
            continue;
        }
        //获得数值
        if (flag) {
            decimalMultiples/=10;
            decimal = decimal + (in[i]-'0')*decimalMultiples;
        }
        else
        {
            num = num*10 + in[i]-'0';
        }
    }
    d->power = (num == 0) ? 1 : (num+decimal);

    if ('-'==in[start]) {
        d->num *= -1;
    }
    for(int i = end; i > start; i--)
    {
        if ('x' == in[i]) {
            break;
        }
        if ('-' == in[i]) {
            d->power *= -1;
            break;
        }
    }
    return d;
}

//比较
double polynoialsCompare(pList a, pList b){
    return ((pPolynomial)a->data)->power - ((pPolynomial)b->data)->power;
}

//检查
pList checkPolynoial(pList p){
    if (0 == ((pPolynomial)p->data)->num) {
        pList t = p->next;
        free(p);
        p = p->next;
    }
    while(NULL != p->next){
        if(0 == ((pPolynomial)p->next->data)->num){
            pList t = p->next;
            p->next = p->next->next;
            t->next = NULL;
            free(t);
        }
        p = p->next;
    }    
}

//排序
pList sortPolynoials(pList list, double (*compare)(pList a, pList b)){
    pList newList = list;
    list = list->next;
    newList->next = NULL;
    while(list!=NULL){
        pList p = list;
        list = list->next;
        p->next = NULL;
        if (compare(p, newList) < 0) {
            p->next = newList;
            newList = p;
            continue;
        }
        if (compare(p, newList) == 0) {
            ((pPolynomial)newList->data)->num += ((pPolynomial)p->data)->num;
            free(p);
            continue;
        }
        pList q = newList;
        int flag = 0;
        while(q->next != NULL){
            if (compare(p, q->next) == 0) {
                ((pPolynomial)q->next->data)->num += ((pPolynomial)p->data)->num;
                free(p);
                flag = 1;
                break;                
            }
            if (compare(p, q->next) > 0) {
                p->next = q->next;
                q->next = p;
                flag = 1;
                break;
            }            
        }
        if (!flag) {
            q->next = p;
        }        
    }
    checkPolynoial(newList);
    return newList;
}

//解析表达式
pList analysisPolynoials(char *in){
    pList list = (pList)malloc(sizeof(pList));
    list->data = NULL;
    list->next = NULL;
    pList p = list;

    int i = 0;
    int flag = 0;
    int start = -1;
    while('\0'!=in[i]){
        if ('+' == in[i]) {
            flag = 1;
        }
        if ('-' == in[i]) {
            if (start+1==i) {
                flag = 0;
            }else{
                int flag2 = 0;
                for(int j = i; j > start; j--)
                {
                    if ('0' <= in[j] && '9'>= in[j]) {
                        flag2 = 1;
                    }
                    if ('x' == in[j]) {
                        if (flag2) {
                            flag = 1;
                        }
                        break;
                    }
                }
            }
        }
        if(flag){
            start = (start < 0) ? 0 :start;
            pList node = (pList)malloc(sizeof(list));
            p->next = node;
            node->data = analysisTerm(in, start, i);
            node->next = NULL;
            start = i;
            p = p->next;
            flag = 0;
        }  
        i++;
    }
    start = (start < 0) ? 0 :start;
    pList node = (pList)malloc(sizeof(list));
    p->next = node;
    node->data = analysisTerm(in, start, i);
    node->next = NULL;

    pList t = list->next;
    list->next = NULL;
    free(list);
    return t;
}

//加法
pList addition(){


    return NULL;
}

//减法
pList subtraction(){

    return NULL;
}

//乘法
pList multiplication(){

    return NULL;
}

//输出
void printPolynomial(pList polynomial){
    if (NULL == polynomial) {
        printf("0\n");
        return;
    }
    int flag = 0;
    while(NULL != polynomial){
        pPolynomial data = (pPolynomial)polynomial->data;
        if (!(data->num < 0) && flag) {
            printf("+");
        }
        printf("%g", data->num);
        if (data->num != 0) {
            if (data->power == 1) {
                printf("x");
            }else if (data->power < 0) {
                printf("x^(%g)", data->power);
            }else if (data->power > 0) {
                printf("x^%g", data->power);
            }
        }
        flag = 1;
        polynomial = polynomial->next;
    }
    printf("\n");
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
    pList result;
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