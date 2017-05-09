/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./copy factor infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    float factor = atof(argv[1]);
    
    if(factor <= 0.0 || factor >100.0)
    {
        fprintf(stderr, "wrong number f\n");
        return 5;
    }
   
    int padding;
    
    if(factor == 1.0)
    {
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    
     padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // write RGB triple to outfile
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }

        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

        // then add it back (to demonstrate how)
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }
    }else
    {
        int height = bi.biHeight;
        int width = bi.biWidth;
        padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;
        RGBTRIPLE triple;
        //allocate memory for temp array
        RGBTRIPLE *temp_array = (RGBTRIPLE *) calloc(width * abs(height), sizeof(RGBTRIPLE));
        if(temp_array == NULL)
        {
            fprintf(stderr, "not enough memmory\n");
            return 6;
        }
        // copy bitmap to temp array
        
        for (int i = 0; i < abs(height); i++)
        {
            for(int j = 0; j < width; j++)
            {
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                temp_array[i * width + j] =  triple;
                
            }
            fseek(inptr, padding, SEEK_CUR);
            
        }
    
        // calculate parameters
        height = round(factor * height);
        width = round(factor * width);
        padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;
        bi.biSizeImage = (sizeof(RGBTRIPLE) * width + padding ) * abs(height);
        bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
        // save previous value of width and height
        int width_prev = bi.biWidth;
        int height_prev = bi.biHeight;
        
        bi.biWidth = width;
        bi.biHeight = height;
        // refresh headers
        fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
        fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);
        // calculate ratio and remainder between sizes
        int ratio_h;
        int ratio_w;
        int remain_h;
        int remain_w;
        height = abs(height);
        height_prev = abs(height_prev);
        if(factor > 1.0)
        {
            ratio_w = width /  width_prev;
            remain_w = width % width_prev;
            ratio_h = height / height_prev;
            remain_h = height % height_prev;
        }
        else
        {
            ratio_w = -width_prev / width;
            remain_w = width_prev % width;
            ratio_h = -height_prev / height;
            remain_h = height_prev % height;
        }
        // iteration for output
        
        int temp_j = 0;
        int temp_i = 0;
        for(int i = 0; i < height; i++)
        {
            int flag_i = (i + height) / (remain_h + height);            // operate additional putting or cutting when we can not divide on whole number
            
            if(ratio_h > 0)                                             // is output bigger?
            {
                temp_i = i / (ratio_h + (flag_i + 1) % 2) - remain_h * ratio_h * flag_i;    // calculation of i-index of initial array
                
            }
            else
            {

                temp_i = i * (abs(ratio_h) + (flag_i + 1) % 2) + remain_h * abs(ratio_h) * flag_i;
                
            }
            // inner cycle
                    for(int j = 0; j < width; j++)
                    {
            
                        int flag_j = (j + width) / (remain_w + width);
                        
                        if(ratio_w > 0)
                        {
                        
                            temp_j = j / (ratio_w +  (flag_j + 1  ) % 2) - remain_w * ratio_w * flag_j;     // calculate j-index of initial array
                        
                        }
                        else
                        {
                            
                            temp_j = j * (abs(ratio_w) + (flag_j + 1) % 2 ) + remain_w* abs(ratio_w) * flag_j;
                            
                        }
                        
                       fwrite(&temp_array[temp_i * width_prev + temp_j], sizeof(RGBTRIPLE), 1, outptr) ;
                       
                    }
                    for (int k = 0; k < padding; k++)
                    {
                    fputc(0x00, outptr);
                    }
            
        }
        free(temp_array);
    }
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
