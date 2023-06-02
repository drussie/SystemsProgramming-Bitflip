/*
	   Student Name: Marcos Ondruska
	   Student ID: 2685885

	   I affirm that I wrote this program without any help from any other
	   people or sources from the internet.

	   Program description: this program will take your integer input and
	   flip the even binary bits, flip the odd binary bits, flip all the bits,
	   as well as switch all the bits from right to left starting at the first 1. If the -o option is selected,
	   the output will be to a file, or otherwise it will be to stdout.

	   example of command line parsing vie bitflip
	   usage: [-e] [-f] [-a] [-s] [-outputfilename] intval

	   Marcos Ondruska
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#define INT_SIZE sizeof(uint16_t)
#define INT_BITS INT_SIZE * 8 - 1

int debug = 0;
// count the number of bits in a binary number
int bitCount(uint16_t input)
{
	int counter = 0;
	while (input > 0)
	{
		counter++;
		input = input >> 1;
	}
	return counter;
}
// find the first 1 from right to left if a number is even
int evenFirstOne(uint16_t num)
{
	int count = 0;
	int i = 0;
	int bit = 0;

	if (num == 0)
	{
		count = 1; // count must be 0 at return
	}
	else
	{
		while (bit != 1)
		{ // Looking for first '1'
			bit = (num >> i) & 1;
			i++;
			count++;
		}
	}
	return count - 1;
}

int main(int argc, char **argv)
{
	extern char *optarg;
	extern int optind;
	int c, err = 0;
	int eflag = 0, fflag = 0, aflag = 0, sflag = 0, oflag = 0; // flags
	char *fname;											   // Initializing fname
	static char usage[] = "usage: %s [-e] [-f] [-a] [-s] [-o outputfilename] intval\n";
	uint16_t userInput, eFlagResult, fFlagResult, aFlagResult, sFlagResult; // initializing user input and temp variables for printing to stdout and to file
	uint64_t inputCheck;													// to check for user input greater than 65535
	char collectArg[6];
	int numberOfBits = 0, bit = 0; // initializing numberOfBits,and bit
	FILE *fptr;					   // initializing file pointer

	while ((c = getopt(argc, argv, "efaso:")) != -1) // options for debug, flipping even bits, fliping odd bits, flipping all bits, switching bits from right to left, and saving output to a file
		switch (c)
		{
		case 'd': // debug
			debug = 1;
			break;
		case 'e': // flip even bits
			eflag = 1;
			break;
		case 'f': // flip odd bits
			fflag = 1;
			break;
		case 'a': // flip all bits
			aflag = 1;
			break;
		case 's': // switch bits right to left
			sflag = 1;
			break;
		case 'o': // save output to file
			oflag = 1;
			fname = optarg;
			break;
		case '?':
			err = 1;
			break;
		}
	if ((optind + 1) > argc)
	{
		/* need at least one argument (change +1 to +2 for two, etc. as needeed) */

		printf("optind = %d, argc=%d\n", optind, argc);
		fprintf(stderr, "%s: missing intval\n", argv[0]);
		fprintf(stderr, usage, argv[0]);
		exit(1);
	}
	else if (err)
	{
		fprintf(stderr, usage, argv[0]);
		exit(1);
	}

	if (optind < argc) /* these are the arguments after the command-line options */
		for (; optind < argc; optind++)
		{
			inputCheck = atoi(argv[optind]); // converting argument string to int
			userInput = inputCheck;
		}
	else
	{
		printf("No arguments left to process\n");
	}

	if (inputCheck > 65535)
	{ // don't want numbers greater than largest 16bit -> 65535
		printf("Please enter a number between 0 and 65535.\n");
		exit(102);
	}

	// Code for switching even bits
	if (eflag == 1)
	{
		uint16_t temp = userInput;
		// for loop using every second bit starting at zero
		for (int i = 0; i < 16; i += 2)
		{
			// setting bit at i
			int bit = (temp >> i) & 1;
			if (bit == 1)
			{
				// removing bit at i
				temp = temp - (bit << i);
			}
			else
			{
				temp = temp + (1 << i); // adding 1 at i
			}
		}
		eFlagResult = temp; // store temp in eFlagResult ready for output
	}
	// code for switching odd bits
	if (fflag == 1)
	{
		uint16_t temp = userInput;
		for (int i = 1; i < 16; i += 2)
		{							   // run loop for every second bit starting at bit 1
			int bit = (temp >> i) & 1; // setting bit at i
			if (bit == 1)
			{
				temp = temp - (bit << i); // removing bit at i
			}
			else
			{
				temp = temp + (1 << i); // adding 1 at i
			}
		}
		fFlagResult = temp; // store temp in fFlagResult ready for output
	}

	if (aflag == 1)
	{
		uint16_t temp = ~(userInput); // flip bits using tilda
		aFlagResult = temp;			  // store temp in aFlagResult ready for output
	}

	if (sflag == 1)
	{
		uint16_t temp = userInput;
		int evenFlag = 0;						// flag if temp is even
		int distanceToOne = evenFirstOne(temp); // find how many 0's to the first 1

		for (int i = 0; i <= INT_BITS; i++)
		{ // loop through the bits starting at 0 and ending at INT_BITS
			if (bit == 1)
			{														 // if bit is odd
				bit = bit | (((temp >> i) & 0x1) << (INT_BITS - i)); // either keeping the bit or grabbing the bit from temp and shifting to INT_BITS - i in bit
			}
			else
			{														 // if bit is even
				bit = bit | (((temp >> i) & 0x1) << (INT_BITS - i)); // either keeping the bit or grabbing the bit from temp and shifting to INT_BITS - i in bit
				evenFlag = 1;
			}
		}
		if (evenFlag == 1)
		{ // add padding 0's
			bit = bit << distanceToOne;
		}
		sFlagResult = bit; // store bit in sFlagResult ready for output
	}
	if (!oflag)
	{
		// if user chooses !oflag, output goes to stdout
		printf("value: %u\n", userInput);
		// Only print selected options
		if (eflag == 1)
		{
			printf("Even bits flipped: %u\n", eFlagResult);
		}
		if (fflag == 1)
		{
			printf("Odd bits flipped: %u\n", fFlagResult);
		}
		if (aflag == 1)
		{
			printf("All bits flipped: %u\n", aFlagResult);
		}
		if (sflag == 1)
		{
			printf("Switched all bits: %d\n", sFlagResult);
		}
	}
	else
	{
		fptr = fopen(fname, "w"); // Open fname (user entered) to write using a filepointer
		if (fptr == NULL)
		{ // something went wrong, print message and exit
			printf("Unable to create oututfile \n");
			exit(100);
		}

		fprintf(fptr, "%u %u %u %d", eFlagResult, fFlagResult, aFlagResult, sFlagResult); // print variables to file
		fclose(fptr);																	  // close file
		printf("File successfully created\n");											  // message to user
	}

	return (0);
}
