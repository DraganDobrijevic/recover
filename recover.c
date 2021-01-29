#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open file %s.\n", argv[1]);
        return 2;
    }

    int i = 0;
    unsigned char *block = malloc(512);
    char fname[8];
    FILE *jpeg_img;

    while (fread(block, 512, 1, inptr))
    {
        if (block[0] == 0xff && block[1] == 0xd8 && block[2] == 0xff && (block[3] & 0xf0) == 0xe0)
        {
            if (i != 0)
            {
                fclose(jpeg_img);
            }

            sprintf(fname, "%03i.jpg", i);
            jpeg_img = fopen(fname, "w");

            if (jpeg_img == NULL)
            {
                free(block);
                fclose(inptr);
                fprintf(stderr, "Could not create output JPG %s", fname);
                return 3;
            }

            i++;
        }

        if (i == 0)
        {
            continue;
        }
        else
        {
            fwrite(block, 512, 1, jpeg_img);
        }

    }

    free(block);
    fclose(inptr);
    fclose(jpeg_img);

    return 0;
}