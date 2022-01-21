#include "bmp.h"

// in bytes
const int FILE_HEADER_SIZE = 14;
const int INFO_HEADER_SIZE = 40;

void generateBitmapImage (unsigned char* image, int height, int width, int bytePerPixel, char* imageFileName)
{
    int widthInBytes = width * bytePerPixel;


    int paddingSize = (4 - (widthInBytes) % 4) % 4;

    int stride = (widthInBytes) + paddingSize;

    int colorByte =  (int) pow(2,bytePerPixel*8)*4; //  [2^ (bit depth) color entries] x 4 (RGB_) bytes/entry

    FILE* imageFile = fopen(imageFileName, "wb");

    unsigned char* fileHeader = createBitmapFileHeader(height, stride, colorByte, bytePerPixel);
    fwrite(fileHeader, 1, FILE_HEADER_SIZE, imageFile);

    unsigned char* infoHeader = createBitmapInfoHeader(height, width, stride,bytePerPixel);
    fwrite(infoHeader, 1, INFO_HEADER_SIZE, imageFile);

    int i;

    if (bytePerPixel == 3){
        unsigned char padding[3] = {0,0,0};
        for (i = 0; i < height; i++) {
            fwrite(image + (i*widthInBytes), bytePerPixel, width, imageFile);
            if(paddingSize != 0){
                fwrite(padding, 1, paddingSize, imageFile);
            }
        }
    }
    else{

        unsigned char *colorTable = (unsigned char*) calloc(colorByte, sizeof(unsigned char));
        fillColorTable(colorTable, colorByte , bytePerPixel);
        fwrite(colorTable, 1, colorByte, imageFile);
        if(bytePerPixel == 2){
            printf("check color table\n");
            for(int k = 1240; k <1250; k ++){
                printf("%X" ,colorTable[0+k*8]); printf(" %X ", colorTable[1+k*8]);
                printf("%X" ,colorTable[2+k*8]); printf(" %X ", colorTable[3+k*8]);
                printf("%X", colorTable[4+k*8]); printf(" %X ", colorTable[5+k*8]);
                printf("%X" ,colorTable[6+k*8]); printf(" %X \n", colorTable[7+k*8]);


            }
        }

        unsigned char padding[3] = {0,0,0};
        for (i = 0; i < height; i++) {
            fwrite(image + (i*widthInBytes), bytePerPixel, width, imageFile);
            if(paddingSize != 0){
                fwrite(padding, 1, paddingSize, imageFile);
            }
        }
    }

    fclose(imageFile);
}

unsigned char* createBitmapFileHeader (int height, int stride, int colorByte ,int bytePerPixel)
{
    int fileSize ;
    int pixelArrayOffset;

    if(bytePerPixel == 3){
        fileSize = FILE_HEADER_SIZE + INFO_HEADER_SIZE  + (stride * height);
        pixelArrayOffset = FILE_HEADER_SIZE + INFO_HEADER_SIZE;

    }
    else {
        fileSize = FILE_HEADER_SIZE + INFO_HEADER_SIZE + colorByte + (stride * height);
        pixelArrayOffset = FILE_HEADER_SIZE + INFO_HEADER_SIZE + colorByte;

    }


    static unsigned char fileHeader[] = {
        0,0,     /// signature
        0,0,0,0, /// image file size in bytes
        0,0,0,0, /// reserved
        0,0,0,0, /// start of pixel array
    };

    fileHeader[ 0] = (unsigned char)('B');
    fileHeader[ 1] = (unsigned char)('M');
    fileHeader[ 2] = (unsigned char)(fileSize      );
    fileHeader[ 3] = (unsigned char)(fileSize >>  8);
    fileHeader[ 4] = (unsigned char)(fileSize >> 16);
    fileHeader[ 5] = (unsigned char)(fileSize >> 24);
    fileHeader[10] = (unsigned char)(pixelArrayOffset);
    fileHeader[11] = (unsigned char)(pixelArrayOffset   >> 8);
    fileHeader[12] = (unsigned char)(pixelArrayOffset   >> 16);
    fileHeader[13] = (unsigned char)(pixelArrayOffset   >> 24);

    return fileHeader;
}

unsigned char* createBitmapInfoHeader (int height, int width, int stride, int bytePerPixel)
{
    static unsigned char infoHeader[] = {
        0,0,0,0, /// header size
        0,0,0,0, /// image width
        0,0,0,0, /// image height
        0,0,     /// number of color planes
        0,0,     /// bits per pixel
        0,0,0,0, /// compression
        0,0,0,0, /// image size
        0,0,0,0, /// horizontal resolution
        0,0,0,0, /// vertical resolution
        0,0,0,0, /// colors in color table (default to 2^bitdepth)
        0,0,0,0, /// important color count
    };

    infoHeader[ 0] = (unsigned char)(INFO_HEADER_SIZE);
    infoHeader[ 4] = (unsigned char)(width      );
    infoHeader[ 5] = (unsigned char)(width >>  8);
    infoHeader[ 6] = (unsigned char)(width >> 16);
    infoHeader[ 7] = (unsigned char)(width >> 24);
    infoHeader[ 8] = (unsigned char)(height      );
    infoHeader[ 9] = (unsigned char)(height >>  8);
    infoHeader[10] = (unsigned char)(height >> 16);
    infoHeader[11] = (unsigned char)(height >> 24);
    infoHeader[12] = (unsigned char)(1);
    infoHeader[14] = (unsigned char)(bytePerPixel*8);

    return infoHeader;
}

void fillColorTable (unsigned char* colorTable, int colorByte, int bytePerPixel){
    int counter = 0;
    int inc = bytePerPixel*4;
    if(bytePerPixel == 1){
        for(int i = 0 ; i < colorByte; i += inc){
            colorTable[i]   = (unsigned char)counter;     // BLUE
            colorTable[i+1] = (unsigned char)counter;   // GREEN
            colorTable[i+2] = (unsigned char)counter;   // RED
            counter++;
        }
    }
    else if (bytePerPixel == 2){
        for(int i = 0 ; i < colorByte; i += inc){
            colorTable[i]   = (unsigned char)(counter);            // BLUE
            colorTable[i+1] = (unsigned char)(counter >> 8);
            colorTable[i+2] = (unsigned char)(counter);          // GREEN
            colorTable[i+3] = (unsigned char)(counter >> 8);
            colorTable[i+4] = (unsigned char)(counter);          // RED
            colorTable[i+5] = (unsigned char)(counter >> 8);
            counter++;
        }
    }
}
