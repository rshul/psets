#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[]){
    if (argc ==1 || argc > 2){                  // check if there is one argument
    printf("error: must be one argument\n");    
    return 1;       
    }
    else {
        int ind = 0;
        while(argv[1][ind] != '\0'){            // else check if all characters are letters
            if(!(isalpha(argv[1][ind++]))) { 
            printf("error: must be all letters\n");
            return 1;
            }
        }
        
    }
    printf("plaintext: ");
    string s = get_string();                    // get string
    
    printf("ciphertext: ");
    int j = 0, len = strlen(argv[1]);
    unsigned char ch;
    
    for(int i = 0, n = strlen(s); i < n;  i++){ // iterate entire message
        if(isalpha(s[i])){                      // if letter
            if(islower(s[i])) {                 // and letter is lowercase    
                ch = ((s[i] + (toupper(argv[1][j % len]) % 'A')) - 'a') % 26 + 'a';         // (sourse character + key char) mod 26 
                printf("%c",ch);
            }else{
                ch = ((s[i] + toupper(argv[1][j % len]) % 'A') - 'A')% 26 + 'A';
                printf("%c",ch);
            }
            j++;
        }else {ch = s[i];printf("%c",ch);}      // just print initial character
        
    }
    printf("\n");
    return 0;
}