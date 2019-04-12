#include <stdio.h>
#include <stdlib.h>
#define MAX_PROCESSES 10

typedef struct
{
    int a1;
    int a2;
}A;

typedef struct
{
    A a;
}B;

int main( int argc, char **argv )
{
    B t;
    t.a.a1=1;
    t.a.a2=2;
    B t2;
    t2.a = t.a;
    printf("a1:%d, a2:%d\n",t2.a.a1,t2.a.a2);
    return 0;
}