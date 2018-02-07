/*Comment placeholder for later.*/

#include <stdio.h>
#include <string.h>

#define FILENAME_LIMIT 50
#define HEADER_SIZE 54
#define ONE_BYTE 1
#define COLORS_PER_PIXEL 3

void invertColors(char *theHeader, int theWidth, unsigned char thePixels[][theWidth * COLORS_PER_PIXEL], int theHeight) {

    FILE* outfile = fopen("InvertedColorCopy.bmp", "wb");
    unsigned char newArray[theHeight][theWidth * COLORS_PER_PIXEL];
    memcpy(newArray, thePixels, (size_t) (theHeight * theWidth * COLORS_PER_PIXEL));

    int r, c;
    for (r = 0; r < theHeight; r++) {
        for (c = 0; c < (theWidth * COLORS_PER_PIXEL); c++/*<--ayy*/) {
            unsigned char temp = newArray[r][c];
            temp = (unsigned char) (255 - temp);
            newArray[r][c] = temp;
        }
    }

    fwrite(theHeader, ONE_BYTE, HEADER_SIZE, outfile);
    fwrite(newArray, ONE_BYTE, (size_t) (theHeight * theWidth * COLORS_PER_PIXEL), outfile);

    fclose(outfile);
}

int main() {

    char fileName[FILENAME_LIMIT];
    int height;
    int width;

    printf("Enter the file name: ");
    scanf("%s", fileName);
    char* file = strcat(fileName, ".bmp");
    printf("Enter the height and width: ");
    scanf("%d %d", &height, &width);

    FILE* infile = fopen(file, "rb");
    char header[HEADER_SIZE];
    unsigned char pixels[height][width * COLORS_PER_PIXEL];
    fread(header, ONE_BYTE, HEADER_SIZE, infile);
    fread(pixels, ONE_BYTE, (size_t) (height * width * COLORS_PER_PIXEL), infile);

    invertColors(header, width, pixels, height);


/*  FILE* outfile = fopen("copy1.bmp", "wb");


    fwrite(header, ONE_BYTE, HEADER_SIZE, outfile);
    fwrite(pixels, ONE_BYTE, (size_t) (height * width * COLORS_PER_PIXEL), outfile);


    fclose(outfile);
*/

    fclose(infile);
    printf("Done. Check the generated images\n");

    return 0;
}

