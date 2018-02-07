/*Comment placeholder for later.*/

#include <stdio.h>
#include <string.h>

#define FILENAME_LIMIT 50
#define HEADER_SIZE 54
#define ONE_BYTE 1
#define COLORS_PER_PIXEL 3

void invertColors(int theHeight, int theWidth, unsigned char thePixels[][theWidth * COLORS_PER_PIXEL], char *theHeader) {

    FILE* outfile = fopen("Copy1.bmp", "wb");
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

void increaseContrast(int theHeight, int theWidth, unsigned char thePixels[][theWidth * COLORS_PER_PIXEL], char *theHeader) {

    FILE* outfile = fopen("Copy2.bmp", "wb");
    unsigned char newArray[theHeight][theWidth * COLORS_PER_PIXEL];
    memcpy(newArray, thePixels, (size_t) (theHeight * theWidth * COLORS_PER_PIXEL));

    int r, c;
    float contrastRatio = 2.9695;
    for (r = 0; r < theHeight; r++) {
        for (c = 0; c < (theWidth * COLORS_PER_PIXEL); c++/*<--ayy*/) {
            int temp = newArray[r][c];
            temp = (int) (contrastRatio * (temp - 128) + 128);
            if (temp < 0) {
                temp = 0;
            } else if (temp > 255) {
                temp = 255;
            }
            newArray[r][c] = (unsigned char) temp;
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
    int or, r, c;
    for (c = 0; c < theWidth * COLORS_PER_PIXEL; c += 3) {
        or = theHeight - 1;
        for (r = 0; r < theHeight; r++) {
            newArray[r][c] = thePixels[or][c];
            newArray[r][c + 1] = thePixels[or][c + 1];
            newArray[r][c + 2] = thePixels[or][c + 2];
            or--;
        }
    }

    /* Mirror: */
    for (r = 0; r < theHeight; r++) {
        int p = 0;
        for (c = theWidth * COLORS_PER_PIXEL - 1; c > theWidth * COLORS_PER_PIXEL / 2; c -= 3) {

            newArray[r][c] = newArray[r][p + 2];
            newArray[r][c - 1] = newArray[r][p + 1];
            newArray[r][c - 2] = newArray[r][p];

            p += 3;
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

    int or, oc, nr, nc;

    nr = theHeight / 2;
    for (or = 0; or < theHeight; or += 2) {
        nc = 0;
        for (oc = 0; oc < theWidth * COLORS_PER_PIXEL; oc += (2 * COLORS_PER_PIXEL)) {
            newArray[nr][nc] = 0;
            newArray[nr][nc + 1] = 0;
            newArray[nr][nc + 2] = thePixels[or][oc];
            nc += 3;
        }
        nr++;
    }

    nr = theHeight / 2;
    for (or = 0; or < theHeight; or += 2) {
        nc = theWidth * COLORS_PER_PIXEL / 2;
        for (oc = 0; oc < theWidth * COLORS_PER_PIXEL; oc += (2 * COLORS_PER_PIXEL)) {
            newArray[nr][nc] = 0;
            newArray[nr][nc + 1] = thePixels[or][oc];
            newArray[nr][nc + 2] = 0;
            nc += 3;
        }
        nr++;
    }

    nr = 0;
    for (or = 0; or < theHeight; or += 2) {
        nc = 0;
        for (oc = 0; oc < theWidth * COLORS_PER_PIXEL; oc += (2 * COLORS_PER_PIXEL)) {
            newArray[nr][nc] = thePixels[or][oc];
            newArray[nr][nc + 1] = 0;
            newArray[nr][nc + 2] = 0;
            nc += 3;
        }
        nr++;
    }

    nr = 0;
    for (or = 0; or < theHeight; or += 2) {
        nc = theWidth * COLORS_PER_PIXEL / 2;
        for (oc = 0; oc < theWidth * COLORS_PER_PIXEL; oc += (2 * COLORS_PER_PIXEL)) {
            newArray[nr][nc] = thePixels[or][oc];
            newArray[nr][nc + 1] = thePixels[or][oc + 1];
            newArray[nr][nc + 2] = thePixels[or][oc + 2];
            nc += 3;
        }
        nr++;
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

    fclose(infile);
    printf("Done. Check the generated images\n");

    return 0;
}

