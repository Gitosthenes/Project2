/*Comment placeholder for later.*/

#include <stdio.h>
#include <string.h>

#define FILENAME_LIMIT 50
#define HEADER_SIZE 54
#define ELEM_SIZE 1
#define COLORS_PER_PIXEL 3

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
    FILE* outfile = fopen("copy1.bmp", "wb");
    char header[54];
    unsigned char pixels[height][width * 3];

    fread(header, ELEM_SIZE, HEADER_SIZE, infile);
    fread(pixels, ELEM_SIZE, (size_t) (height * width * COLORS_PER_PIXEL), infile);

    fwrite(header, ELEM_SIZE, HEADER_SIZE, outfile);
    fwrite(pixels, ELEM_SIZE, (size_t) (height * width * COLORS_PER_PIXEL), outfile);

    fclose(infile);
    fclose(outfile);

    printf("Done. Check the generated images\n");

    return 0;
}