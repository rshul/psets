#include <cs50.h>
#include <stdio.h>
int main(void){
    int n;
    printf("Minutes: ");
    do{
        n = get_int();
       
    }while (n<0);
    printf("%i\nBottles: %i",n,n*12);
    
}