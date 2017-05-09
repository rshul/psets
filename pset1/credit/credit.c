# include <stdio.h>

# include <cs50.h>

int main(void)
{
    long long num;
    printf("Number: ");
    do
    {
        num = get_long_long();
    }while(num <= 0);
    
    int count = 0;
    int sum = 0;
    long long i;
    for( i = 1; i <= num; i *= 10)
    {
        int cur_num = (num / i) % 10;
        int flag = count % 2;
        sum = sum + cur_num * ((flag + 1) % 2) 
            + ((cur_num << 1) % 10 + (cur_num << 1) / 10) * flag;
        count++; 
    }
    i /= 100;
    
    if(sum % 10)
    {
        printf("INVALID\n");
        return 0;
    }
    else if(num / (i * 10) == 4 
            && (count == 13 || count == 16))
    {
        printf("VISA\n");
    }
    else if(num / i > 50 && num / i < 56 && count == 16)
    {
        printf("MASTERCARD\n");
    }
    else if((num / i == 34 || num / i == 37) && count == 15)
    {
        printf("AMEX\n");
    }
    else printf("INVALID\n");
    
    
    return 0;
}
