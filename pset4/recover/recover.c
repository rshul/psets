#include <stdio.h>
#include <stdlib.h>

struct block 
{
    unsigned char first;
    unsigned char second;
    unsigned char third;
    unsigned char fourth;
    unsigned char data[508];
    
} bl;

int main(int argc, char *argv[])
{
    
    if(argc != 2)
    {
        fprintf(stderr, "Usage: recover [name of file]\n");
        return 1;
    }
    int count = 0;
    FILE *file = fopen(argv[1],"r");

    if(file == NULL)
    {

        fprintf(stderr, "can't opent file\n");
        return 2;
        
    }
    char name[8];
    int result;
    FILE *fout;
    do
    {
        result = fread(&bl, sizeof(struct block), 1, file);
        
        if(bl.first == 0xff && bl.second == 0xd8 
            && bl.third == 0xff && (bl.fourth & 0xf0) == 0xe0 )
        {
            
                if(count != 0) fclose(fout);
                 sprintf(name,"%03i.jpg", count);
                 
                fout = fopen(name, "w");
                if(fout == NULL)
                {
                    fprintf(stderr, "can't open file\n");
                    return 3;
                }
                
                count++;
              
        }
        
        if(count != 0 && result == 1) fwrite(&bl, sizeof(struct block), 1, fout);

        
    }while(result == 1);
    
    fclose(file);
    fclose(fout);
    return 0;
}