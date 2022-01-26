/*
	* Poke actuators individually
*/

#include "BMCApi.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <time.h>

#include <unistd.h>
#include <termios.h>

char getch(void);	// read SIG IO

typedef int bool_t;	// boolean type definition
#ifndef TRUE
#define TRUE	1
#endif
#ifndef FALSE
#define FALSE	0
#endif

const char* serial_number = "17BW025#079";

void Sleep_us(double us)
{
	usleep((unsigned int)us);
}

/*!
 * \brief test_poke Poke actuators one by one using BMCSetArray()
 * \param hdm DM handle
 * \param map_lut Driver channel mapping from BMCLoadMap()
 * \param delay Delay between actuators in continuous mode.
 * \return Error code
 */
BMCRC test_poke(DM *hdm, uint32_t *map_lut, double delay)
{

	uint32_t	k = 0;
	double	*test_array;
  BMCRC	rv = NO_ERR;
	int		err_count = 0;
	char	ch = '\0';

	printf("Will begin continuous poke test with delay %.0f ms.\n", delay/1000);

	// initialize a command vector
	test_array = (double *)calloc(hdm->ActCount, sizeof(double));

	for(k=0; k<hdm->ActCount; k++) {
		// set command of kth actuator to 0.5
		test_array[k] = 0.5;

		// send the command vector to the DM
		rv = BMCSetArray(hdm, test_array, map_lut);

		if(rv) {
			printf("\nError %d poking actuator %d.\n",rv,k);
			err_count++;
		} else {
			printf("\rPoked actuator %d.", k);
			fflush(stdout);
		}

		test_array[k] = 0; // clear the command of kth actuator
		Sleep_us(delay);
	}
		printf("\n");
		printf("Poke test complete. %d errors.\n", err_count);
    free(test_array);
		return rv;
}

int main(int argc, char* argv[])
{

	DM hdm;											// DM handle
	BMCRC rv;										// return value from BMC APIs
	double *test_array1;				// command vector
	double *test_array2;
  uint32_t *map_lut;					// mapping array: 1-D vector to 2-D map of actuators


	int select = 0;
	int err_count = 0;					 // error count for the entire program
	int pistons = 10;						// number actuators used in the initial test
	int delay_us = 500000;					// microsec
  char ch = '\0';							// store IO input

	bool_t auto_exit = FALSE;

	// Open driver
  memset(&hdm, 0, sizeof(hdm));
	rv = BMCOpen(&hdm, serial_number);		// initialize the DM

	if(rv) {
		printf("Error %d opening the driver type %u.\n", rv, (unsigned int)hdm.Driver_Type);
		printf("%s\n\n", BMCErrorString(rv));

        if (!auto_exit) {
            printf("Press any key to exit.\n");
			getch();
        }
		return rv;
	}

	printf("Opened Device %d with %d actuators.\n", hdm.DevId, hdm.ActCount);

  // Allocate and initialize control arrays.
  map_lut		= (uint32_t *)malloc(sizeof(uint32_t)*MAX_DM_SIZE);
	test_array1	= (double *)calloc(hdm.ActCount, sizeof(double));		// push command
	test_array2 = (double *)calloc(hdm.ActCount, sizeof(double));		// pull command

	int k;
	for(k=0; k<(int)hdm.ActCount; k++) {
		map_lut[k] = 0;
		test_array1[k] = 0.5;
		test_array2[k] = 0;
	}

	// Open default actuator map from disk
	rv = BMCLoadMap(&hdm, NULL, map_lut);

	// Send array of voltages to DM
	printf("Will begin continuous piston test with delay %d ms.\n", delay_us/1000);
	printf("Press 'x' to end test.\n");

	// initial test
	for(k=0; k < pistons; k++) {
		Sleep_us((double)delay_us);


		// Alternate between the two piston values
		switch(select) {
			case 0:
				rv = BMCSetArray(&hdm, test_array1, map_lut);
				select = 1;
				break;
			case 1:
				rv = BMCSetArray(&hdm, test_array2, map_lut);
				select = 0;
				break;
			default:
				select = 0;
		}

		if(rv) {
			err_count++;
			printf("\nError %d sending voltages.\n", rv);
		} else {
			printf("\rSent array %d of %d.", k , pistons);
			fflush(stdout);
		}
	}

	printf("\n");
	printf("Piston Test complete. %d arrays sent and %d errors.\n\n", (k-err_count), err_count);

	// Clear array
	rv = BMCClearArray(&hdm);
	if(rv)
		printf("Error %d clearing voltages.\n", rv);

	// Set each actuator individually
	rv = test_poke(&hdm, map_lut, (double)delay_us);

	// Clear array and close.
	rv = BMCClearArray(&hdm);
	if (rv)
		printf("Error %d clearing voltages.\n", rv);

	rv = BMCClose(&hdm);
	if (rv)
		printf("Error %d closing the driver.\n", rv);

	// clean up
	free(test_array1);
	free(test_array2);
	free(map_lut);

    if (!auto_exit) {
        printf("Tests complete. Press any key to exit.\n");
		getch();
    }

	return 0;
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
