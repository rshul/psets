#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>
#define LETTERS 52

int calc_comb(int size_of_psw);
int find_psw(int size, string argv[], char psw_temp[]);

int main(int argc, string argv[]){                  
    if(argc < 2 || argc > 2){                               // verify if there is one argument
        printf("error: must be one argument\n");
        return 1;
    }
    
    
    char p2[3] = {'a','a','\0'};                            // 2 elements size of password
    char p3[4] = { 'a','a','a','\0'};                       // 3 elements size of password
    char p4[5] = { 'a','a','a','a','\0'};                   // 4 elements size of password
    
    int result = find_psw(2,argv, p2);                      // crack password!
    
    if(!result) printf("%s\n", p2);
    else{
        result = find_psw(3,argv,p3);
        if(!result) printf("%s\n",p3);
        else{
            result = find_psw(4,argv,p4);
            if(!result) printf("%s\n",p4);
            else printf("not found\n");
        }
    }
    
    return 0;
}
/**
* calculating combinations;
* define total combinations for definite size of password
*/
int calc_comb(int size_of_psw){
    int comb = 1;
    for(int i = 0; i < size_of_psw; i++){
        comb *= LETTERS; 
    }
    return comb;
}
/**
 * looking for proper password;
 * compare every combination of letters to find suitable
 * 
 */
int find_psw(int size, string argv[], char psw_temp[]){
    char salt[3];
    strncpy(salt, argv[1], 2);
    salt[2]='\0';
        
    int combinations = calc_comb(size);
    string encr_psw;
    int count[4] = {0,0,0,0};
    int first_letter[4] = {0,0,0,0};
    int k[4] = {0,0,0,0};
    char psw[5] = {'a','a','a','a','\0'};
    
    for(int i = 0; i < combinations; i++ ){                 // iterate every combination
        k[0] = i % (LETTERS / 2);
        
        count[0] = i/(LETTERS / 2);
        count[1] = count[0] / LETTERS;
        count[2] = count[1] / LETTERS;
        count[3] = count[2] / LETTERS;
        k[1] = (count[0] /2) % 26;
        k[2] = (count[1] /2) % 26;
        k[3] = (count[2] /2) % 26;

        first_letter[0] = 'a' -  (count[0] % 2) * 32;
        first_letter[1] = 'a' - (count[1] % 2) * 32;
        first_letter[2] = 'a' - (count[2] % 2) * 32;
        first_letter[3] = 'a' - (count[3] % 2) * 32;
        
        psw[0] = first_letter[0] + k[0];
        psw[1] = first_letter[1] + k[1];
        psw[2] = first_letter[2] + k[2];
        psw[3] = first_letter[3] + k[3];
        
        for(int j=0; j < size; j++){
            psw_temp[j] = psw[j];
        }
        encr_psw = crypt(psw_temp,salt);
        if(!strcmp(argv[1],encr_psw)) return 0;             // compare if there is result
    }
    return 1;
    
}