/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "dictionary.h"

node *htable[HTABLE_SIZE] = {NULL};         // hash table

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    char lcword[LENGTH + 1];
    
    for (int i = 0; word[i] != '\0'; i++ )      // convert word to lowercase
    lcword[i]= tolower(word[i]);
    lcword[strlen(word)] = '\0';                
    
    
        node *cursor = htable[hfunc(lcword, HTABLE_SIZE)];
        while(cursor != NULL)                               // checking
        {
            
            if(!strcmp(lcword, cursor->word)) return true;
            cursor = cursor->next;
        }

    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    char word[LENGTH + 1];
    /*
    for(int i = 0; i < HTABLE_SIZE; i++)
    {
        htable[i] = NULL;
        
    }
    */
    FILE *file = fopen(dictionary,"r");
    if(file == NULL)
    {
        fprintf(stderr, "can't open file %s\n", dictionary);
        return false;
    }
    
    while(fscanf(file, "%s", word) != EOF)
    {
        
        
        node *temp_node = malloc(sizeof(node));
        if(temp_node == NULL)
        {
            fclose(file);
            unload();
            return false;
        }
        strcpy(temp_node->word, word);
    
        unsigned long i_ht = hfunc(temp_node->word,HTABLE_SIZE); // get hash index

        temp_node->next = htable[i_ht];
        htable[i_ht] = temp_node;
        
    }
    fclose(file);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    unsigned int count = 0;
    for(int i = 0; i < HTABLE_SIZE; i++)
    {
        node *cursor = htable[i];
        while(cursor != NULL)
        {
            count++;
            cursor = cursor->next;
        }
        
    }
    return count;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    bool emptytab = false;
    for (int i = 0; i < HTABLE_SIZE; i++)
    {
        if(htable[i] != NULL 
        && emptytab == false ) 
        emptytab = true;
        
        node *cursor = htable[i];
        while(cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
            
        }
    }
    return emptytab;
}

/**
 * http://www.cse.yorku.ca/~oz/hash.html
 * djb2
 * this algorithm (k=33) was first reported by dan bernstein many years ago in comp.lang.c. ; 
 * the magic of number 33 (why it works better than many other constants, prime or not) 
 * has never been adequately explained.
 * 
 * 
 * 
 */

unsigned long hfunc(char *str, int size)
{
    unsigned long hash = 5381;
        int c;

        while ((c = *str++))
            hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

        return hash % size;
    
}