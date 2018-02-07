/* Name:        Alex Bledsoe
 *
 * Environment: Tested on Mint 18.3 Desktop 32-bit
 *
 * Comments:    Using bmpchange.c as a foundation, my logic was to take in the specified file based on
 *              what the user input, process it into 2 arrays (one for the header and a 2d array for the pixel data),
 *              then pass those arrays off to four separate functions, each of which would manipulate the image in
 *              a different way.
 *
 *              Said functions would each make a copy of the 2d array for manipulation so that the original would be
 *              intact when passing it to the next function. Then they would do the manipulation, write the new pixel
 *              data to a .bmp file and close the file stream.
 *
 * Xtra Credit: I added a function called pixelGradient that starts with the image in full resolution on the left, but
 *              gradually becomes more pixelated as you move to the right.
 *
 *              In reality I came up with a single algorithm to pixelate the image. take the color values of a single
 *              pixel in the original image and change the pixels around it to match, essentially giving the appearance
 *              that each pixel is larger that it is. I took this and applied it to the new image in fourths, increasing
 *              the pixelation effect  each time. The first quarter was full resolution. In the second quarter, the
 *              pixels are 4 times as large, 8 times in the next quarter, and 16 times in the last.
 *
 */

#include <stdio.h>
#include <string.h>

#define FILENAME_LIMIT 50
#define HEADER_SIZE 54
#define ONE_BYTE 1
#define COLORS_PER_PIXEL 3

void pixelGradient(int theHeight, int theWidth, unsigned char thePixels[][theWidth * COLORS_PER_PIXEL], char *theHeader) {

    FILE* outfile = fopen("PixelGradient.bmp", "wb");
    unsigned char newArray[theHeight][theWidth * COLORS_PER_PIXEL];
    memcpy(newArray, thePixels, (size_t) (theHeight * theWidth * COLORS_PER_PIXEL));

    int oldRow, oldColumn, newRow, newColumn;

    newRow = 0;
    for (oldRow = 0; oldRow < theHeight; oldRow += 2) {
        newColumn = theWidth * COLORS_PER_PIXEL * 1/4;
        for (oldColumn = theWidth * COLORS_PER_PIXEL * 1/4; oldColumn < theWidth * COLORS_PER_PIXEL; oldColumn += (2 * COLORS_PER_PIXEL)) {
            unsigned char blue = thePixels[oldRow][oldColumn];
            unsigned char green = thePixels[oldRow][oldColumn + 1];
            unsigned char red = thePixels[oldRow][oldColumn + 2];

            int i, j;
            for (i = 0; i < 2; i++) {
                for (j = 0; j < 6; j += 3) {
                    newArray[newRow + i][newColumn + j] = blue;
                    newArray[newRow + i][newColumn + j + 1] = green;
                    newArray[newRow + i][newColumn + j + 2] = red;
                }
            }
            newColumn += 6;
        }
        newRow += 2;
    }

    newRow = 0;
    for (oldRow = 0; oldRow < theHeight; oldRow += 4) {
        newColumn = theWidth * COLORS_PER_PIXEL * 1/2;
        for (oldColumn = theWidth * COLORS_PER_PIXEL * 1/2; oldColumn < theWidth * COLORS_PER_PIXEL; oldColumn += (4 * COLORS_PER_PIXEL)) {
            unsigned char blue = thePixels[oldRow][oldColumn];
            unsigned char green = thePixels[oldRow][oldColumn + 1];
            unsigned char red = thePixels[oldRow][oldColumn + 2];

            int i, j;
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 12; j += 3) {
                    newArray[newRow + i][newColumn + j] = blue;
                    newArray[newRow + i][newColumn + j + 1] = green;
                    newArray[newRow + i][newColumn + j + 2] = red;
                }
            }
            newColumn += 12;
        }
        newRow += 4;
    }

    newRow = 0;
    for (oldRow = 0; oldRow < theHeight; oldRow += 8) {
        newColumn = theWidth * COLORS_PER_PIXEL * 3/4;
        for (oldColumn = theWidth * COLORS_PER_PIXEL * 3/4; oldColumn < theWidth * COLORS_PER_PIXEL; oldColumn += (8 * COLORS_PER_PIXEL)) {
            unsigned char blue = thePixels[oldRow][oldColumn];
            unsigned char green = thePixels[oldRow][oldColumn + 1];
            unsigned char red = thePixels[oldRow][oldColumn + 2];

            int row, column;
            for (row = 0; row < 8; row++) {
                for (column = 0; column < 24; column += 3) {
                    newArray[newRow + row][newColumn + column] = blue;
                    newArray[newRow + row][newColumn + column + 1] = green;
                    newArray[newRow + row][newColumn + column + 2] = red;
                }
            }
            newColumn += 24;
        }
        newRow += 8;
    }

    fwrite(theHeader, ONE_BYTE, HEADER_SIZE, outfile);
    fwrite(newArray, ONE_BYTE, (size_t) (theHeight * theWidth * COLORS_PER_PIXEL), outfile);

    fclose(outfile);
}

void invertColors(int theHeight, int theWidth, unsigned char thePixels[][theWidth * COLORS_PER_PIXEL], char *theHeader) {

    FILE* outfile = fopen("Copy1.bmp", "wb");
    unsigned char newArray[theHeight][theWidth * COLORS_PER_PIXEL];
    memcpy(newArray, thePixels, (size_t) (theHeight * theWidth * COLORS_PER_PIXEL));

    int row, column;
    for (row = 0; row < theHeight; row++) {
        for (column = 0; column < (theWidth * COLORS_PER_PIXEL); column++) {
            unsigned char temp = newArray[row][column];
            temp = (unsigned char) (255 - temp);
            newArray[row][column] = temp;
        }
    }

    fwrite(theHeader, ONE_BYTE, HEADER_SIZE, outfile);
    fwrite(newArray, ONE_BYTE, (size_t) (theHeight * theWidth * COLORS_PER_PIXEL), outfile);

    fclose(outfile);
}

void increaseContrast(int theHeight, int theWidth, unsigned char thePixels[][theWidth * COLORS_PER_PIXEL], char *theHeader) {

    FILE* outfile = fopen("Copy2.bmp", "wb");
    unsigned char newArray[theHeight][theWidth * COLORS_PER_PIXEL];
    memcpy(newArray, thePixels, (size_t) (theHeight * theWidth * COLORS_PER_PIXEL));

    int row, column;
    float contrastRatio = 2.9695;
    for (row = 0; row < theHeight; row++) {
        for (column = 0; column < (theWidth * COLORS_PER_PIXEL); column++) {
            int temp = newArray[row][column];
            temp = (int) (contrastRatio * (temp - 128) + 128);
            if (temp < 0) {
                temp = 0;
            } else if (temp > 255) {
                temp = 255;
            }
            newArray[row][column] = (unsigned char) temp;
        }
    }

    fwrite(theHeader, ONE_BYTE, HEADER_SIZE, outfile);
    fwrite(newArray, ONE_BYTE, (size_t) (theHeight * theWidth * COLORS_PER_PIXEL), outfile);

    fclose(outfile);
}

void flipAndMirror(int theHeight, int theWidth, unsigned char thePixels[][theWidth * COLORS_PER_PIXEL], char *theHeader) {

    FILE* outfile = fopen("Copy3.bmp", "wb");
    unsigned char newArray[theHeight][theWidth * COLORS_PER_PIXEL];
    memcpy(newArray, thePixels, (size_t) (theHeight * theWidth * COLORS_PER_PIXEL));

    /* Flip: */
    int oldRow, row, column;
    for (column = 0; column < theWidth * COLORS_PER_PIXEL; column += 3) {
        oldRow = theHeight - 1;
        for (row = 0; row < theHeight; row++) {
            newArray[row][column] = thePixels[oldRow][column];
            newArray[row][column + 1] = thePixels[oldRow][column + 1];
            newArray[row][column + 2] = thePixels[oldRow][column + 2];
            oldRow--;
        }
    }

    /* Mirror: */
    for (row = 0; row < theHeight; row++) {
        int startOfPixel = 0;
        for (column = theWidth * COLORS_PER_PIXEL - 1; column > theWidth * COLORS_PER_PIXEL / 2; column -= 3) {

            newArray[row][column] = newArray[row][startOfPixel + 2];
            newArray[row][column - 1] = newArray[row][startOfPixel + 1];
            newArray[row][column - 2] = newArray[row][startOfPixel];

            startOfPixel += 3;
        }
    }

    fwrite(theHeader, ONE_BYTE, HEADER_SIZE, outfile);
    fwrite(newArray, ONE_BYTE, (size_t) (theHeight * theWidth * COLORS_PER_PIXEL), outfile);

    fclose(outfile);
}

void scaleDown(int theHeight, int theWidth, unsigned char thePixels[][theWidth * COLORS_PER_PIXEL], char *theHeader) {

    FILE* outfile = fopen("Copy4.bmp", "wb");
    unsigned char newArray[theHeight][theWidth * COLORS_PER_PIXEL];
    memcpy(newArray, thePixels, (size_t) (theHeight * theWidth * COLORS_PER_PIXEL));

    int oldRow, oldColumn, newRow, newColumn;

    /* Red copy in top-left. */
    newRow = theHeight / 2;
    for (oldRow = 0; oldRow < theHeight; oldRow += 2) {
        newColumn = 0;
        for (oldColumn = 0; oldColumn < theWidth * COLORS_PER_PIXEL; oldColumn += (2 * COLORS_PER_PIXEL)) {
            newArray[newRow][newColumn] = 0;
            newArray[newRow][newColumn + 1] = 0;
            newArray[newRow][newColumn + 2] = thePixels[oldRow][oldColumn];
            newColumn += 3;
        }
        newRow++;
    }

    /* Green copy in top-right. */
    newRow = theHeight / 2;
    for (oldRow = 0; oldRow < theHeight; oldRow += 2) {
        newColumn = theWidth * COLORS_PER_PIXEL / 2;
        for (oldColumn = 0; oldColumn < theWidth * COLORS_PER_PIXEL; oldColumn += (2 * COLORS_PER_PIXEL)) {
            newArray[newRow][newColumn] = 0;
            newArray[newRow][newColumn + 1] = thePixels[oldRow][oldColumn];
            newArray[newRow][newColumn + 2] = 0;
            newColumn += 3;
        }
        newRow++;
    }

    /* Blue copy in bottom-left. */
    newRow = 0;
    for (oldRow = 0; oldRow < theHeight; oldRow += 2) {
        newColumn = 0;
        for (oldColumn = 0; oldColumn < theWidth * COLORS_PER_PIXEL; oldColumn += (2 * COLORS_PER_PIXEL)) {
            newArray[newRow][newColumn] = thePixels[oldRow][oldColumn];
            newArray[newRow][newColumn + 1] = 0;
            newArray[newRow][newColumn + 2] = 0;
            newColumn += 3;
        }
        newRow++;
    }

    /* Full copy in bottom-right. */
    newRow = 0;
    for (oldRow = 0; oldRow < theHeight; oldRow += 2) {
        newColumn = theWidth * COLORS_PER_PIXEL / 2;
        for (oldColumn = 0; oldColumn < theWidth * COLORS_PER_PIXEL; oldColumn += (2 * COLORS_PER_PIXEL)) {
            newArray[newRow][newColumn] = thePixels[oldRow][oldColumn];
            newArray[newRow][newColumn + 1] = thePixels[oldRow][oldColumn + 1];
            newArray[newRow][newColumn + 2] = thePixels[oldRow][oldColumn + 2];
            newColumn += 3;
        }
        newRow++;
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

    invertColors(height, width, pixels, header);
    increaseContrast(height, width, pixels, header);
    flipAndMirror(height, width, pixels, header);
    scaleDown(height, width, pixels, header);
    pixelGradient(height, width, pixels, header);

    fclose(infile);
    printf("Done. Check the generated images\n");

    return 0;
}