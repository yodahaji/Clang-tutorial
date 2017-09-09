#define L_AND(a, b) a && b
#define L_OR(a, b) a || b

#include<stdio.h>
// Begin function swap returning integer
int swap(int a,int b)
{
    if(a==b)
    {
      return 1;
    }
    int temp;
    temp=a;
    a=b;
    b=temp;
    return 2;
}
// End function swap

// Begin function add returning integer
int add(int a,int b)
{
    return a+b;
}
// End function add

// Begin function main returning integer
int main(){
    int a;
    int b;
    int c;
    printf("hello world");
    return 0;
}
// End function main
