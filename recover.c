#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc < 1)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Remember filenames
    char *infile = argv[1];

    // Open input file for reading
    FILE *raw_file = fopen(infile, "rb");
    if (raw_file == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 1;
    }

    // For naming output image
    char z = '0';
    int img = 0;
    char *name = malloc(8);

    // Start coping image files
    int stflag = 0;
    BYTE buffer[BLOCK_SIZE]; // Temp to copy
    FILE *outptr;            // Initialaze file pointer
    while (fread(buffer, 1, BLOCK_SIZE, raw_file) == BLOCK_SIZE)
    {
        // Find when image begins
        if (buffer[0] == 255 && buffer[1] == 216 && buffer[2] == 255 && buffer[3] >= 224 && buffer[3] <= 239)
        {

            // Skip the increment in the first loop
            if (stflag == 1)
            {
                fclose(outptr);
                img++;
            }

            // Name and open the output file close privious output file
            if (img < 10)
            {
                sprintf(name, "%c%c%i.jpg", z, z, img);
            }
            else if (img < 100)
            {
                sprintf(name, "%c%i.jpg", z, img);
            }
            else if (img < 1000)
            {
                sprintf(name, "%i.jpg", img);
            }
            // printf("Creating %s\n", name);

            outptr = fopen(name, "wb");
            if (outptr == NULL)
            {
                printf("Could not open %s.\n", name);
                fclose(raw_file);
                return 1;
            }
            // First loop Started
            stflag = 1;
        }

        // Copy
        if (stflag == 1)
        {
            fwrite(buffer, 1, BLOCK_SIZE, outptr);
        }
    }

    // Close all files
    fclose(raw_file);
    fclose(outptr);
    free(name);

    return 0;
}