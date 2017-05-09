#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void){
    string s = get_string();
    if( s != NULL){
        int i = 0;
        if(s[0] != ' ') printf("%c",toupper(s[0]));
        else i++;
        while(s[i] != '\0'){
              while(s[i] == ' ') i++;
              
              if(s[i-1] == ' ' && s[i] != '\0') printf("%c",toupper(s[i]));
              i++;
        } 
        printf("\n");
        
        
    }
    
    return 0;
}