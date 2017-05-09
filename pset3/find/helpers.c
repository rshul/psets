/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>

#include "helpers.h"
#define MAX_COUNT 65536
/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    int left = 0;
    int right = n-1;
    int middle;
    while(left<= right)
    {
        middle = (left + right)/2;
        if(values[middle] == value) return true;
        else if(values[middle] > value) right = middle - 1;
        else if(values[middle] < value) left = middle + 1;
        
        
        
    }
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    int temp_array[MAX_COUNT] = {0};
    for(int i = 0; i < n; i++)
    {
        temp_array[values[i]]++;
    }
    int count = 0;
    for(int i = 0; i < MAX_COUNT; i++)
    {
        if(temp_array[i] > 0)
        {
            while(temp_array[i] > 0)
            {
                values[count++] = i;
                temp_array[i]--;
            }
            
        }
        
    }
    return;
}
