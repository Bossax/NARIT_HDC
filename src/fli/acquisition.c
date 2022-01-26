// CCD resolution 1600x 1200 WxH
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>

#include "libfli.h"
#include "libfli-camera.h"
#include "libfli-mem.h"
#include "libfli-camera-usb.h"
#include "bmp.h"

char getch(void);	// read SIG IO


#define MAX_STR_LENGTH 50
#define MAX_DEVICE 2

const long max_width = 1600;
const long max_height = 1200;
const flibitdepth_t bitdepth = FLI_MODE_8BIT;
const long exposureTime = 200; //msec
const int bytePerPixel = 1;
const long width = 800;
const long height = 800;
const double target_temperature = -20.0;

int main(int argc, char const *argv[]) {
    int err;
    flidev_t dev;		// dev handle
	char file[MAX_STR_LENGTH], name[MAX_STR_LENGTH];
	char *listName[MAX_DEVICE];
	long listDomain[MAX_DEVICE];
	int numCams = 0;
	long domain;

	char ch;

	// Enumerate devices
	FLICreateList(FLIDOMAIN_USB | FLIDEVICE_CAMERA);

	if(FLIListFirst(&domain, file, MAX_STR_LENGTH, name, MAX_STR_LENGTH) == 0)
	{
	do
	{
		listName[numCams] = (char*)malloc(strlen(file) + 1);
		strcpy(listName[numCams], file);

		listDomain[numCams] = domain;
		numCams++;
	}
	while((FLIListNext(&domain, file, MAX_STR_LENGTH, name, MAX_STR_LENGTH) == 0) && (numCams < MAX_DEVICE));
	}

	FLIDeleteList();

	// Open cameras
	if(numCams == 0)
	{
	printf("\nNo FLI cameras have been detected\n");
	exit(0);
	}
	printf("%d camera(s) detected,\n", numCams);
	printf("%s is connected to %lX\n",listName[0], listDomain[0]);

	if((err = FLIOpen(&dev, listName[0], listDomain[0])))
	{
		printf( "Error FLIOpen: error %d\n", err);
		exit(1);
	}

    // Get the model of the device
    char model[MAX_STR_LENGTH];
    if ((err = FLIGetModel(dev, model, MAX_STR_LENGTH))){

        fprintf(stderr, "Error FLIOpen: %s\n", strerror((int)-err));
    }
    printf("Device model %s\n", model);


    // Set exposure time
    if ((err = FLISetExposureTime(dev, exposureTime))){
        fprintf(stderr, "Error FLISetExposureTime: %s\n", strerror((int)-err));
    }
	printf("Exposre time is set to %ld msec\n", exposureTime );

	// Set ROI
	long upper_x = (max_width - width)/2;
	long upper_y = (max_height - height)/2;
	long lower_x = upper_x + width;
	long lower_y = upper_y + height;

	if((err = FLISetImageArea(dev, upper_x, upper_y, lower_x,lower_y))){
		fprintf(stderr, "Error FLISetImageArea: %s\n", strerror((int)-err));
	}
	printf("ROI is set\n" );

	// Wait for the temperature to reach the desire value
	if((err = FLISetTemperature(dev, target_temperature))){
		fprintf(stderr, "Error FLISetTemperature: %s\n", strerror((int)-err));
	}
	printf("Target Temperature is set to %f\n", target_temperature );
	printf("Please wait for the camera to reach the target temperature\n");
	printf("Press X to cancle waiting and proceed:\n");
	double temp = 0.0;

	FLIGetTemperature(dev, &temp);
	printf("Current temperature = %f\n", temp );
	do {

		FLIGetTemperature(dev, &temp);

		ch = getch();
		if ('X' == toupper(ch))
				break;

	} while (temp > target_temperature+0.5);

//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
	printf("Proceed to taking images\n");
	printf("Press X to terminate the program \n" );
	int widthInByte = width * bytePerPixel;
	int buffsize = widthInByte * height;

    // Take an image
    unsigned char imgBuffer[height][width];
	memset(&imgBuffer, 0, buffsize);
	void* row_ptr = calloc(widthInByte, sizeof(unsigned char));;
	long timeleft = 10;

	while(1){
		FLIExposeFrame(dev);
		FLIGetExposureStatus(dev,&timeleft);

		while( timeleft > 0){
			FLIGetExposureStatus(dev,&timeleft);
		}

	    for(int row = 0; row < height; row++){
	        if ((err = FLIGrabRow(dev, row_ptr, width))){
	            fprintf(stderr, "Error FLIGrabRow: %s\n", strerror((int)-err));
	        }
	        memcpy(imgBuffer+row, (unsigned char*)(row_ptr), widthInByte);
	    }
		printf("Image is taken\n");

		ch = getch();
		if ('X' == toupper(ch))
				break;

	}

    generateBitmapImage((unsigned char*) imgBuffer, (int) height, (int) width, bytePerPixel, "FLI_img.bmp" );

	free(imgBuffer);
	free(row_ptr);
	return 1;


}

// signal driven I/O
// read terminal input and return the character
char getch(void) {
	char buf = 0;
	struct termios old = {0};
	if (tcgetattr(0, &old) < 0)
			perror("tcsetattr()");
	old.c_lflag &= ~ICANON;
	old.c_lflag &= ~ECHO;
	old.c_cc[VMIN] = 1;
	old.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSANOW, &old) < 0)
			perror("tcsetattr ICANON");
	if (read(0, &buf, 1) < 0)
			perror ("read()");
	old.c_lflag |= ICANON;
	old.c_lflag |= ECHO;
	if (tcsetattr(0, TCSADRAIN, &old) < 0)
			perror ("tcsetattr ~ICANON");
	return (buf);
}
