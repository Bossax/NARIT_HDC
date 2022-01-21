#include <stdio.h>
#include <string.h>

#include "libfli.h"
#include "libfli-camera.h"
#include "libfli-mem.h"
#include "libfli-camera-usb.h"
#include <stdlib.h>
#include "bmp/bmp.h"

#define MAX_STR_LENGTH 100
const flibitdepth_t bitdepth = FLI_MODE_8BIT;
const long exposureTime = 10; //msec
const int bytePerPixel = 1;

int main(int argc, char const *argv[]) {
    int err;
    flidev_t dev;
    flidomain_t domain = FLIDOMAIN_USB;
    char** names;


    //List available device connected to the specific communication channel
     if ((err = FLIList(domain, &names))){
         fprintf(stderr, "Error FLIList: %s\n", strerror((int)-err));

     }
     // Null pointer
     if(!names[0]){
         printf("No devices.. exit\n");
         exit(0);
     }

    char* name = names[0];
    printf("Device name %s\n", name);

    // Open the device with corresponding name
    if ((err = FLIOpen(&dev, name, domain))){

        fprintf(stderr, "Error FLIOpen: %s\n", strerror((int)-err));
    }

    // Get the model of the device
    char model[MAX_STR_LENGTH];
    if ((err = FLIGetModel(dev, model, MAX_STR_LENGTH))){

        fprintf(stderr, "Error FLIOpen: %s\n", strerror((int)-err));
    }
    printf("Device model %s\n", model);

    //Set bitdepth
    if ((err = FLISetBitDepth(dev, bitdepth))){

        fprintf(stderr, "Error FLIOpen: %s\n", strerror((int)-err));
    }

    // Set exposure time
    if ((err = FLISetExposureTime(dev, exposureTime))){

        fprintf(stderr, "Error FLIOpen: %s\n", strerror((int)-err));
    }


    // get camera visible area
    long left, top, right ,bottom;
    if ((err = FLIGetVisibleArea(dev, &left, &top, &right, &bottom))){

        fprintf(stderr, "Error FLIGetVisibleArea: %s\n", strerror((int)-err));
    }

    // get image size
    long ul_x, ul_y, lr_x, lr_y;
    if ((err = FLIGetArrayArea(dev, &ul_x, &ul_y, &lr_x, &lr_y))){

        fprintf(stderr, "Error FLIGetArrayArea: %s\n", strerror((int)-err));
    }
    /// area info
    printf("=== Area Info. === \n");
    printf("Visible Area: \n");
    printf("Upper Left = (%ld,%ld), Lower Right = (%ld,%ld)",   left,  top, right, bottom);
    printf("Total Area: \n");
    printf("Upper Left = (%ld,%ld), Lower Right = (%ld,%ld)",   ul_x, ul_y, lr_x, lr_y);


    // calculate image size
    long width = lr_x - ul_x;
    long widthInByte = width * bytePerPixel;
    long height = lr_y - ul_y;
    long area = width*height;
    size_t buffsize = (size_t) (area * bytePerPixel);

    // Take an image
    char* imgBuffer = (char*) calloc(buffsize, sizeof(char));
    void* row_ptr = calloc(widthInByte, sizeof(char));
    for(int row = 0; row < height; row++){
        if ((err = FLIGrabRow(dev, row_ptr, width))){
            fprintf(stderr, "Error FLIOpen: %s\n", strerror((int)-err));
        }
        *(imgBuffer+row*widthInByte) = *((char*) (row_ptr));
    }

    generateBitmapImage((unsigned char*) imgBuffer, height, width, bytePerPixel, "FLI_img.bmp" );


}
