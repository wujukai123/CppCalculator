#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <string>
#define maxsize 1024

typedef struct
{
    float data[maxsize];
    int top;
}Stack1;

void Initstack1(Stack1 *S)
{
    S = (Stack1*)malloc(sizeof(Stack1));
    S->top = -1;
}

int Push1(Stack1 * S, float ch)
{
    if (S->top == maxsize - 1)
        return 0;
    else
    {
        S->top++;
        S->data[S->top] = ch;
        return 1;
    }
}

int Pop1(Stack1 * S, float ch)
{
    if (S->top == -1)
        printf("栈上溢出!\n");
    else
        ch = S->data[S->top];
    S->top--;
    return 1;
}

typedef struct
{
    char data[maxsize];
    int top;
}Stack2;

void Initstack2(Stack2 *S)
{
    S = (Stack2*)malloc(sizeof(Stack2));
    S->top = -1;
}

int Push2(Stack2 *S, char ch)
{
    if (S->top == maxsize - 1)
        return 0;
    else
    {
        S->top++;
        S->data[S->top] = ch;
        return 1;
    }
}

int Pop2(Stack2 *S, char ch)
{
    if (S->top == -1)
        printf("栈上溢出!\n");
    else
        ch = S->data[S->top];
    S->top--;
    return 1;
}

int Comop(char ch)
{
    switch (ch)
    {
    case'+':
    case'-':
    case'*':
    case'/':
    case'(':
    case')':
    case'#':
        return 1;
    default:
        return 0;
    }
}

char Prior[7][7] =
{
          // '+' '-'   '*'  '/'  '('  ')'  '#'    
    /*'+'*/{ '>', '>', '<', '<', '<', '>', '>' },
    /*'-'*/{ '>', '>', '<', '<', '<', '>', '>' },
    /*'*'*/{ '>', '>', '>', '>', '<', '>', '>' },
    /*'/'*/{ '>', '>', '>', '>', '<', '>', '>' },
    /*'('*/{ '<', '<', '<', '<', '<', '=', '< ' },
    /*')'*/{ '>', '>', '>', '>', ' ', '>', '>' },
    /*'#'*/{ '<', '<', '<', '<', '<', '> ', '=' },
};

int Opid(char op1)
{
    switch (op1)
    {
    case'+':return 0;
    case'-':return 1;
    case'*':return 2;
    case'/':return 3;
    case'(':return 4;
    case')':return 5;
    case'#':return 6;
    default:return -123456;
    }
}

char Precede(char op1, char op2)
{
    int a, b;
    a = Opid(op1); b = Opid(op2);
    return(Prior[a][b]);
}

float Operation(float a, char op, float b)
{
    switch (op)
    {
    case '+':
        return b + a;
    case '-':
        return b - a;
    case '*':
        return b * a;
    case '/':
        return b / a;
    default:
        return -123456;
    }
}

void CreateExpression(char *exp)
{

    if (exp == NULL)
    {
        exit(1);
    }
    scanf("%s", exp);
}

void TransmitExpression(char * exp, char postexp[])
{
    Stack2 FZ;
    Initstack2(&FZ);
    int i = 0;
    char x;
    FZ.top = -1;
    Push2(&FZ, '#');
    FZ.data[FZ.top] = '#';
    while (*exp != '\0')
    {
        if (!Comop(*exp))
        {
            while (*exp >= '0'&&*exp <= '9')//读取一个数字串
            {
                postexp[i++] = *exp;
                exp++;
            }
            postexp[i++] = '#';
        }
        else
            switch (Precede(FZ.data[FZ.top], *exp))
        {
            case'<':
                Push2(&FZ, *exp);
                exp++;
                break;
            case'=':
                x = FZ.data[FZ.top];
                Pop2(&FZ, x);
                exp++;
                break;
            case'>':
                postexp[i++] = FZ.data[FZ.top];
                x = FZ.data[FZ.top];
                Pop2(&FZ, x);
                break;
        }
    }
    while (FZ.data[FZ.top] != '#')
    {
        postexp[i++] = FZ.data[FZ.top];
        x = FZ.data[FZ.top];
        Pop2(&FZ, x);
    }
    postexp[i] = '\0';
}

float EvaluateExpression(char * postexp)
{
    Stack1 SZ;
    Initstack1(&SZ);
    float a, b, d;
    SZ.top = -1;
    while (*postexp != '\0')
    {
        switch (*postexp)
        {
        case'+':
        case'-':
        case'*':
        case'/':
            a = SZ.data[SZ.top];
            Pop1(&SZ, a);
            b = SZ.data[SZ.top];
            Pop1(&SZ, b);
            Push1(&SZ, Operation(a, *postexp, b));
            break;
        default:
            d = 0;
            while (*postexp >= '0'&&*postexp <= '9')
            {
                d = 10 * d + *postexp - '0';
                postexp++;
            }
            Push1(&SZ, d);
            SZ.data[SZ.top] = d;

            break;
        }
        postexp++;
    }
    return(SZ.data[SZ.top]);
}

int Error(char *exp)
{
    int i = 0;
    while (exp[i] != '\0')
    {
        if (
            ((exp[i] == '+' || exp[i] == '-' || exp[i] == '*' || exp[i] == '/') && (exp[i + 1] == ')'))
            || ((exp[i] == '+' || exp[i] == '-' || exp[i] == '*' || exp[i] == '/') && (exp[i - 1] == '('))
            || (exp[i] == ')'&&exp[i + 1] == '(')
            || (exp[i] == '('&&exp[i + 1] == ')')
            || ((exp[i] == ')') && exp[i + 1] >= '0'&&exp[i + 1] <= '9')
            || (exp[i] >= '0'&&exp[i] <= '9'&&exp[i + 1] == '(')
            || (exp[0] == '+' || exp[0] == '-' || exp[0] == '*' || exp[0] == '/' || exp[0] == ')')
            || ((exp[i] == '+' || exp[i] == '-' || exp[i] == '*' || exp[i] == '/') && (exp[i + 1] == '+' || exp[i + 1] == '-' || exp[i + 1] == '*' || exp[i + 1] == '/'))
            || (exp[i] == '/'&&exp[i + 1] == '0')
            )
            return 1;
        else
        if (exp[i] == '\0')
            return 0;
        i++;
    }
    return 0;
}

void menu()
{
	printf("\t\t Mini Calculator 1.0 Developer Beta for C++\n");
    printf("\t\t————————————————————————————————————————————\n");
    printf("\t\t|                  计算器                  |\n");
    printf("\t\t|              ----------------            |\n");
    printf("\t\t|              |              |            |\n");
    printf("\t\t|              ----------------            |\n");
    printf("\t\t|              1    2    3   +             |\n");
    printf("\t\t|              4    5    6   -             |\n");
    printf("\t\t|              7    8    9   *             |\n");
    printf("\t\t|              0   （    ）  /             |\n");
    printf("\t\t————————————————————————————————————————————\n");
    printf("\t\t 请输入你要进行的操作：\n");
    printf("\t\t a.表达式求值 b.清空 c.退出\n");
}
void clear()
{
    system("cls");
    menu();
}
void Quit()
{
    system("cls");
    exit(1);
}

int main()
{
    char c;
    char exp[maxsize];
    char postexp[maxsize] = { 0 };
    menu();
    while (1)
    {
        scanf("%c", &c);
        switch (c)
        {
        case 'a':
        sr :
            printf("请输入要计算的表达式:\n");
           CreateExpression(exp);
           if (!Error(exp))
           {
               TransmitExpression(exp, postexp);
               printf("后缀表达式为：%s\n", postexp);
               printf("表达式结果为：%s=", exp);
               printf("%g\n", EvaluateExpression(postexp));
               break;
           }
           else
           if (Error(exp))
           {
               printf("您输入的表达式有误！\n");
               goto sr;
           }
        case'b':
            clear();
            break;
        case'c':
            Quit();
            break;
        }
    }
    system("pause");
}