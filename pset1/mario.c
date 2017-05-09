#include<stdio.h>
#include<cs50.h>
int main(void){
    int n;
    
    do{
        printf("Height: "); 
        n = get_int();
       if(n==0) return 0;
        //printf("\n");
    }while(n<0 || n>23);
    for(int i=0; i<n;i++){
        for(int j = 0; j<n-i-1; j++) printf(" "); for(int j=0; j<i+1; j++) printf("#");
        printf("  ");
        for(int j = 0; j<i+1; j++) printf("#");
        printf("\n");
    }
    
}