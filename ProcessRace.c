#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

#define PIPE_NAME "NAME_OF_PIPE"

// Function prototypes
void writeColours(int, char*);
void printProgress(int, int, int);
void printFinalMessage(char*, int);

int main(void)
{
	// variable declaration
	int fd;
	pid_t pidRED, pidGRN, pidBLU, pidRead;

	// Create the named pipe
	mknod(PIPE_NAME, S_IFIFO | 0666, 0);

	// Opens the named pipe for both reading and writing
	fd = open(PIPE_NAME, O_RDWR | O_NDELAY );

	// Child process that writes 'RED' to the pipe
	pidRED = fork();
	if (pidRED == 0)
	{
		printf("Pid %d (red) started\n", getpid());
		writeColours(fd, "RED"); // Write to the pipe
		exit(1); // ends the child process
	}

	// Child process that writes 'GRN' to the pipe
	pidGRN = fork();
	if (pidGRN == 0)
	{
		printf("Pid %d (green) started\n", getpid());
		writeColours(fd, "GRN"); // Writes to the pipe
		exit(1); // ends the child process
	}

	// Child process that writes 'BLU' to the pipe
	pidBLU = fork();	
	if (pidBLU == 0)
	{
		printf("Pid %d (blue) started\n", getpid());
		writeColours(fd, "BLU"); // Writes to the pipe
		exit(1); // ends the child process
	}

	// Child process that reads the data from the pipe
	// and tallies up the totals
	pidRead = fork();
	if (pidRead == 0)
	{
		// Variables to keep track of totals written by each process
		int nbrOfRed = 0, nbrOfGreen = 0, nbrOfBlue = 0;

		// Variable to hold the data read in from the pipe
		char data[4];

		// Calculate the system time 5 seconds from now
		time_t endTime = time(0) + 5;

		// Runs loop for 5 seconds
		while(time(0) < endTime)
		{
			// Reads in the data from the pipe
			read(fd, &data, 4);

			// Determines which process wrote to the pipe
			// Increments the variable that's tallying the total
			if (!strcmp(data, "RED"))
			{
				nbrOfRed++;
			}
			else if (!strcmp(data, "GRN"))
			{
				nbrOfGreen++;
			}
			else if (!strcmp(data, "BLU")) 
			{
				nbrOfBlue++;
			}

			// Determine how many seconds the process has left to run
			double oldDiff = difftime(endTime, time(0));
			double newDiff;

			// The difference in how many seconds the process has left
			// Will change every 1 second
			// This is used as a measure to know when to print
			// Out the progress of the race
			while( newDiff != oldDiff && oldDiff != 0)
			{				
				printProgress(nbrOfRed, nbrOfGreen, nbrOfBlue);
				newDiff = difftime(endTime, time(0));
				break;
			} 

		}

		// Close the pipe
		close(fd);

		// Print the final messages for each process
		printf("\n"); // line break
		printFinalMessage("red", nbrOfRed);
		printFinalMessage("green", nbrOfGreen);
		printFinalMessage("blue", nbrOfBlue);
	} 

	return 0;
}

// This function will loop for 5 seconds and perform 2 steps
// 1) Sleep 10 milliseconds
// 2) Write a colour to the pipe
void writeColours(int fd, char* colour)
{

	time_t endTime = time(0) + 5;

	// Runs for 5 seconds
	while(time(0) < endTime)
	{
		sleep(0.01); // sleeps for 10 milliseconds, 0.01 seconds
		write(fd, colour, strlen(colour) + 1); // writes the data to the pipe
	}

}

// Determines which process is in the lead
// Prints out a statement showing the standings of the race
// e.g. 'Blue is ahead with 100, red is behind wth 98, green is last with 102!'
void printProgress(int nbrOfRed, int nbrOfGreen, int nbrOfBlue)
{
	char *first, *second, *third;
	int firstNum, secondNum, thirdNum;

	// Determine which process is in first placse
	if ( (nbrOfRed > nbrOfGreen) && (nbrOfRed > nbrOfBlue) )
	{
		first = "Red";
		firstNum = nbrOfRed;
	} 
	else if ( (nbrOfGreen > nbrOfRed) && (nbrOfGreen > nbrOfBlue) )
	{
		first = "Green";
		firstNum = nbrOfGreen;
	}
	else
	{		
		first = "Blue";
		firstNum = nbrOfBlue;
	}

	// Determine which process is in second place
	if ( ((nbrOfRed > nbrOfGreen) && (nbrOfRed < nbrOfBlue)) || ((nbrOfRed < nbrOfGreen) && (nbrOfRed > nbrOfBlue)) )
	{
		second = "Red";
		secondNum = nbrOfRed;
	} 
	else if ( ((nbrOfGreen > nbrOfRed) && (nbrOfGreen < nbrOfBlue)) || ((nbrOfGreen < nbrOfRed) && (nbrOfGreen > nbrOfBlue)) )
	{
		second = "Green";
		secondNum = nbrOfGreen;
	}
	else
	{		
		second = "Blue";
		secondNum = nbrOfBlue;
	}

	// Determine which process is in third place
	if ((nbrOfRed < nbrOfGreen) && (nbrOfRed < nbrOfBlue))
	{
		third = "Red";
		thirdNum = nbrOfRed;
	} 
	else if ((nbrOfGreen < nbrOfRed) && (nbrOfGreen < nbrOfBlue))
	{
		third = "Green";
		thirdNum = nbrOfGreen;
	}
	else 
	{		
		third = "Blue";
		thirdNum = nbrOfBlue;
	}

	printf("\n%s is ahead with %d, %s is behind wth %d, %s is last with %d!", first, firstNum, second, secondNum, third, thirdNum);
}	

// Prints out the final message
// e.g 'Red: I was doing step number 5001, I hope I won!
void printFinalMessage(char* colour, int total)
{
	printf("\n%s: I was doing step number %d, I hope I won!", colour, total);
}
