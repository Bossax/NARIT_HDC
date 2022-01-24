#ifndef _BMP_H_
#define _BMP_H_
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void generateBitmapImage(unsigned char* image, int height, int width, int bytePerPixel, char* imageFileName);
unsigned char* createBitmapFileHeader(int height, int stride, int colorByte,int bytePerPixel);
unsigned char* createBitmapInfoHeader(int height, int width, int stride, int bytePerPixel);
void  fillColorTable (unsigned char* colorTable ,int colorByte, int bytePerPixel);
unsigned char* createRGBA32Mask();
#endif
