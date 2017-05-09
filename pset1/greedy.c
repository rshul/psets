#include<stdio.h>
#include<cs50.h>
#include<math.h>
int main(void){
    float n;
    int coin, count = 0;

    printf("O hai! ");
    do{
        printf("How much change is owed?\n");
        n = get_float();
    
                    
    }while(n < 0.0);
    n = n*100;
    
   coin = round(n);
    while(coin % 25 < coin) {
        count++;
        coin -= 25;
    }
     while(coin % 10 < coin) {
        count++;
        coin -= 10;
    }
     while(coin % 5 < coin) {
        count++;
        coin -= 5;
    }
     while(coin % 1 < coin) {
        count++;
        coin -= 1;
    }
    printf("%i\n",count);
    return 0;
}