#include "bmp.h"
#include <stdlib.h>
const int RGB_PIXEL = 3; /// red, green, & blue
const int PIXEL_8BPP = 1;
const int PIXEL_16BPP = 2;


int main ()
{
    int height = 502;
    int width = 812;
    unsigned char imgRGB[height][width][RGB_PIXEL];
    char* imgRGBFileName = (char*) "bitmapimgRGB.bmp";

    unsigned char imgGB_8bbp[height][width];
    char* imgGB_8bbpFileName = (char*) "bitmapimgGB_8bbp.bmp";

    unsigned char imgGB_16bbp[height][width][PIXEL_16BPP];
    char* imgGB_16bbpFileName = (char*) "bitmapimgGB_16bbp.bmp";



    // Generating RGB imgRGB sample
    int i, j;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            imgRGB[i][j][2] = (unsigned char) ( i * 255 / height );             ///red
            imgRGB[i][j][1] = (unsigned char) ( j * 255 / width );              ///green
            imgRGB[i][j][0] = (unsigned char) ( (i+j) * 255 / (height+width) ); ///blue

        }
    }
    // Generating Grayscale image 8 bbp
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            // imgGB[i][j] = (unsigned char) round(rand() * 255 /2000000 );
            imgGB_8bbp[i][j] = (unsigned char) ( (i+j) * 255 / (height+width) );

        }
    }

    // Generating Grayscale image 8 bbp
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            int val = (i+j) / (height+width)* 65536;
            imgGB_16bbp[i][j][1] = (unsigned char) (val >> 8);
            imgGB_16bbp[i][j][0] = (unsigned char) (val);

        }
    }
    generateBitmapImage((unsigned char*) imgRGB, height, width, RGB_PIXEL, imgRGBFileName);
    generateBitmapImage((unsigned char*) imgGB_8bbp, height, width, PIXEL_8BPP, imgGB_8bbpFileName);
    generateBitmapImage((unsigned char*) imgGB_16bbp, height, width, PIXEL_16BPP, imgGB_16bbpFileName);


    printf("images generated!!\n");
}
