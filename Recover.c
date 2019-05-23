#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    if (argv[1] == NULL)
    {
        return 1;
    }
    // remember filenames
    char *infile = argv[1];

    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 1;
    }

    // in between save location for bytes form card
    unsigned char buffer[512];

    int c = 0;
    bool firstJpeg = true;
    bool readData = false;
    bool newJpeg = true;
    char fileName[50];
    FILE *img = NULL;

    // infinite loop, until break
    while (fread(buffer, 512, 1, inptr))
    {
        // Check if beginning of jpeg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            newJpeg = true;

            // Is it the very first jpg on the card?
            if (firstJpeg)
            {
                firstJpeg = false;

                // We open a new file to write into
                sprintf(fileName, "%03i.jpg", c);
                img = fopen(fileName, "w");

                if (img == NULL)
                {
                    printf("img == NULL");
                    return 2;
                }
            }
            else
            {
                // If it's not the first jpeg, we have to close the previous one
                fclose(img);
                sprintf(fileName, "%03i.jpg", c);
                img = fopen(fileName, "w");

                if (img == NULL)
                {
                    printf("img == NULL");
                    return 2;
                }
            }
            // set flag for the next operation and increment
            readData = true;
            c++;
        }

        if (readData)
        {
            // Take the Data from our buffer, and write it into the newly opened jpeg file
            fwrite(&buffer, 512, 1, img);
        }
    }
}
