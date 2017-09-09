#include<stdio.h>
int swap(int a,int b)
{
    if(a==b)
      return 1;
    int temp;
    temp=a;
    a=b;
    b=temp;
    return 2;
}
int add(int a,int b)
{
    return a+b;
}
int main(){
    int a;
    int b;
    int c;
    printf("hello world");
    return 0;
}