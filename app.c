/**
 * GitHub：https://github.com/1459177541/Polynomial-operation
 **/
#include<stdio.h>
#include<malloc.h>
/**
 * 多项式
 * */
typedef struct polynomial
{
    double num;
    double power;
}polynomial, *pPolynomial;
/**
 * 链表
 * */
typedef struct list
{
    void *data;
    struct list *next;
}list, *pList;

/**
 * 从屏幕输入（一行）
 * return：输入的结果
 * */
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

/**
 * 输出表达式
 * polynomial ： 输出的多项式，data数据为polynomial
 * */
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
        if (1 == data->num && 0 == data->power) {          
            printf("1");
        }else if (-1 == data->num && 0 != data->power){
            printf("-");
        }else{
            printf("%g", data->num);  
        }
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

/**
 * 解析一个
 * 可解析以下格式多项式：
 *      3x^5,-3x,0.3x^(-5),5,...
 * in : 字符串
 * start ： 开始的偏移量
 * end ： 结束的偏移量
 * return：解析结果
 * */
pPolynomial analysisTerm(char *in, int start, int end){
    pPolynomial d = (pPolynomial)malloc(sizeof(polynomial));
    int num = 0;
    double decimal = 0;
    double decimalMultiples = 1;
    int decimalFlag = 0;    //切换小数
    int numFlag = 1;            //切换幂
    for(int i = start; i < end; i++)
    {
        //切换到x的指数
        if ('x'==in[i]) {
            d->num = num+decimal;
            if (0 == d->num && in[i-1] != '0') {
                d->num = 1;
            }            
            num = 0;
            decimal = 0;
            decimalMultiples = 1;
            decimalFlag = 0;
            numFlag = 0;
            continue;
        }
        //切换到小数
        if ('.' == in[i]) {
            decimalFlag = 1;
            continue;
        }
        //非法字符
        if ('0'>in[i] || '9'<in[i]) {
            continue;
        }
        //获得数值
        if (decimalFlag) {
            decimalMultiples/=10;
            decimal = decimal + (in[i]-'0')*decimalMultiples;
        }
        else{
            num = num*10 + in[i]-'0';
        }
    }
    //是否输入x的指数
    if (numFlag) {
        d->num = num+decimal;
        d->power = 0;
    }
    else{
        d->power = (num == 0) ? 1 : (num+decimal);  
    }
    //设置系数的符号
    if ('-'==in[start]) {
        d->num *= -1;
    }
    //设置x的指数的符号
    for(int i = end-1; i >= start; i--){
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

/**
 * 多项式比较
 * a ： 比较的第一个多项式
 * b ： 比较的第二个多项式
 * return：a>b:返回一个正数;a=b：返回0;a<b：返回一个负数
 * */
double polynomialCompare(pList a, pList b){
    return ((pPolynomial)a->data)->power - ((pPolynomial)b->data)->power;
}

/**
 * 检查多项式，并将num为0的多项式移除
 * p ： 待检查的多项式，data的类型为polynomial
 * return：检查后的结果，data的类型为polynomial
 * */
pList checkPolynoial(pList p){
    while (0 == ((pPolynomial)p->data)->num) {
        pList t = p->next;
        free(p);
        p = t;
        if (NULL == p) {
            return NULL;
        }    
    }
    pList t = p;
    while(NULL != p->next){
        if(0 == ((pPolynomial)p->next->data)->num){
            pList t = p->next;
            p->next = p->next->next;
            t->next = NULL;
            free(t);
            continue;
        }
        p = p->next;
    }   
    return t; 
}

/**
 * 使用插入排序将多项式按power排序，并将power相等的多项式进行合并
 * list：排序前的多项式，data的类型为polynomial
 * return： 排序结果，data的类型为polynomial
 * */
pList sortPolynomial(pList list){
    if (NULL == list) {
        return NULL;
    }    
    pList newList = list;
    list = list->next;
    newList->next = NULL;
    while(list!=NULL){
        pList p = list;
        list = list->next;
        p->next = NULL;
        double comp = polynomialCompare(p, newList);
        if (comp < 0) {
            p->next = newList;
            newList = p;
            continue;
        }
        if (comp == 0) {
            ((pPolynomial)newList->data)->num += ((pPolynomial)p->data)->num;
            free(p);
            continue;
        }
        pList q = newList;
        int flag = 0;
        while(q->next != NULL){
            comp = polynomialCompare(p, q->next);
            if (comp == 0) {
                ((pPolynomial)q->next->data)->num += ((pPolynomial)p->data)->num;
                free(p);
                flag = 1;
                break;                
            }
            if (comp < 0 && polynomialCompare(p,q) > 0) {
                p->next = q->next;
                q->next = p;
                flag = 1;
                break;
            }            
            q = q->next;
        }
        if (!flag) {
            q->next = p;
        }        
    }
    newList = checkPolynoial(newList);
    return newList;
}

/**
 * 解析一行多项式，每行多项式为单个多项式相通过加减运算获得
 * return：解析的结果，data类型为polynomial
 * */
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
                if ('^'!= in[i-1] && '(' != in[i-1]) {
                    flag = 1;
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
    t = checkPolynoial(t);
    return t;
}

/**
 * 从文件获取表达式
 * symbol：显示符号
 * stop：输入终止符
 * return：输入的结果，data类型为list
 * */
pList inputByFile(char * symbol, char stop){
    printf("请输入文件名：");
    char* fileName = (char*)malloc(80*sizeof(char));
    scanf("%s", fileName);
    FILE *f = fopen(fileName, "r");
    free(fileName);
    char* buffer = (char*)malloc(120*sizeof(char));
    pList list = (pList)malloc(sizeof(list));
    list->data = NULL;
    list->next = NULL;
    pList p = list;
    while(1){
        if (feof(f)) {
            break;
        }        
        fscanf(f, "%s", buffer);
        printf("\t%s\n", buffer);
        if ('\0' == buffer[0] || (stop == buffer[0] && '\0' == buffer[1])) {
            break;
        }
        printf(symbol);
        pList n = (pList)malloc(sizeof(list));
        n->data = analysisPolynoials(buffer);
        n->next = NULL;
        p->next = n;
        p = p->next;
        buffer[0] = '\0';
    }
    fclose(f);
    free(buffer);
    pList t = list->next;
    free(list);
    return t;
}

/**
 * 从屏幕获取表达式
 * symbol：显示符号
 * stop：输入终止符
 * return：输入的结果，data类型为list
 * */
pList inputPolynoials(char * symbol, char stop){
    printf("请输入多个多项式，通过行分隔，直接输入回车或者输入%c结束\n", stop);
    char * in;
    pList list = (pList)malloc(sizeof(list));
    list->data = NULL;
    list->next = NULL;
    pList p = list;
    while(1){
        printf("\t");
        in = input();
        if ('\0' == in[0] || (stop == in[0] && '\0' == in[1])) {
            break;
        }
        printf(symbol);
        pList n = (pList)malloc(sizeof(list));
        n->data = analysisPolynoials(in);
        n->next = NULL;
        p->next = n;
        p = p->next;
        free(in);
        in = "";
    }
    pList t = list->next;
    free(list);
    return t;
}

/**
 * 多项式的加法
 * list：每行多项式的链表，data数据为多项式list
 * return：计算结果，data类型为polynomial
 * */
pList addition(pList list){
    if (NULL == list) {
        return NULL;
    }    
    pList result = (pList)list->data;
    while(NULL == result){
        list = list->next;
        if(NULL == list){
            return NULL;
        }
        result = (pList)list->data;
    }
    pList p = result;
    list = list->next;
    while(NULL != list){
        while(NULL != p->next){
            p=p->next;
        }
        p->next = list->data;
        list=list->next;
    }    
    return sortPolynomial(result);
}

/**
 * 多项式的减法
 * list：每行多项式的链表，data类型为list
 * return：计算结果，data类型为polynomial
 * */
pList subtraction(pList list){
    if (NULL == list) {
        return NULL;
    }    
    pList result = (pList)list->data;
    while(NULL == result){
        list = list->next;
        if(NULL == list){
            return NULL;
        }
        result = (pList)list->data;
    }
    pList p = result;
    while(NULL != p->next){
        p = p->next;
    }
    list = list->next;
    while(NULL != list){
        p->next = list->data;
        while(NULL != p->next){
            p = p->next;
            ((pPolynomial)p->data)->num *= -1;
        }
        list = list->next;
    }    
    return sortPolynomial(result);
}

/**
 * 两个多项式相乘
 * poly1：第一个多项式
 * poly2：第二个多项式
 * return：计算结果，data类型为polynomial
 * */
pList multiplicationBy2(pList poly1, pList poly2){
    pList temp = (pList)malloc(sizeof(list));
    pList tp = temp;
    while(NULL != poly2){
        int num = ((pPolynomial)poly2->data)->num;
        int power = ((pPolynomial)poly2->data)->power;
        pList tpp = (pList)malloc(sizeof(list));
        pList tppp = tpp;
        pList p1p = poly1;
        while(NULL != p1p){
            pPolynomial d = (pPolynomial)malloc(sizeof(polynomial));
            d->num = num * ((pPolynomial)p1p->data)->num;
            d->power = power + ((pPolynomial)p1p->data)->power;
            pList tpppn = (pList)malloc(sizeof(list));
            tpppn->data = d;
            tpppn->next = NULL;
            tppp->next = tpppn;
            tppp = tppp->next;
            p1p = p1p->next;
        }
        pList t = tpp;
        tpp = tpp->next;
        free(t);
        pList tpn = (pList)malloc(sizeof(list));
        tpn->data = tpp;
        tpn->next = NULL;
        tp->next = tpn;
        tp = tp->next;
        poly2 = poly2->next;
    }
    pList t = temp;
    temp = temp->next;
    free(t);
    return sortPolynomial(addition(temp));
}

/**
 * 多项式的乘法
 * list：每行多项式的链表，data的类型为list
 * return：计算结果，data类型为polynomial
 * */
pList multiplication(pList list){
    if (NULL == list) {
        return NULL;
    }    
    pList result = (pList)list->data;
    list = list->next;
    while(NULL != list){
        result = multiplicationBy2(result, list->data);
        list = list->next;
    }    
    return result;
}

/**
 * 菜单
 * */
void menu(){
    printf(" ----菜单----\n");
    printf("1.多项式相加(输入)\n");
    printf("2.多项式相减(输入)\n");
    printf("3.多项式相乘(输入)\n");
    printf("4.多项式相加(文件)\n");
    printf("5.多项式相减(文件)\n");
    printf("6.多项式相乘(文件)\n");
    printf("0.退出\n");
    printf("请输入您的选择:");
    char in = getchar();
    while('\n'!=getchar()){}
    pList result;
    pList list;
    switch (in){
        case '1':
            list = inputPolynoials("+", '0');
            result = addition(list);
            break;
        case '2':
            list = inputPolynoials("-", '0');
            result = subtraction(list);
            break;
        case '3':
            list = inputPolynoials("*", '1');
            result = multiplication(list);
            break;
        case '4':
            list = inputByFile("+", '0');
            result = addition(list);
            break;
        case '5':
            list = inputByFile("-", '0');
            result = subtraction(list);
            break;
        case '6':
            list = inputByFile("*", '1');
            result = multiplication(list);
            break;
        case '0':
            return;
        default:
            printf("输入错误，请选择正确选项！\n");
            menu();
            return;
    }
    printf("--------------------------------------------------------------\n");
    printf("=\t");
    printPolynomial(result);
    while(NULL != list){
        pList p = list;
        list = list->next;
        free(p);
    }    
    printf("按回车键返回菜单\n");
    while('\n'!=getchar()){}
    menu();
}

/**
 * 主方法
 * */
int main(){
    menu();
    printf("感谢使用!\n");
    return 0;
}