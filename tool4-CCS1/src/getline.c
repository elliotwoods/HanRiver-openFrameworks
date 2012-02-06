#include "stdio.h"
#include "stdlib.h"
#include "string.h"

/** ccs
 * replacing getline on os-x, this is a useful bit of code
 */
int getlineCCS(char** s, size_t *length, FILE *fptr);

/**
 * takes an unallocated string s, (which is free'd if 
 * it is allocated, and reads from fptr until the
 * next "\n"
 * returns -1 if we get EOF otherwise returns the
 * number of char's read
 *
 * if we get EOF the internal buffer will be dealloc'ed
 * but the incoming buffer (s) will not be, be warned
 */
int getlineCCS(char** s, size_t *length, FILE *fptr){
	int count = 0;
	// this is weird, as used in track2root the buffer never reallocs,
	// instead it silently overruns and fucks up, whwwwwyyy?
	int bufferSize = 256;
	char *buffer = NULL;
	char inChar;

	buffer = malloc(sizeof(char)*bufferSize);
	if(buffer == NULL){
		fprintf(stderr, "getline cannot malloc a buffer?\n");
		exit(1);
	}

	if(fptr == NULL){
		fprintf(stderr, "getline called with bad fptr\n");
		exit(1);
	}
	
	do{
		inChar = getc(fptr);
		if(feof(fptr)){
			fprintf(stderr, "getline got EOF\n");
			// tidy up here, (should we free *s, no we should not)
			free(buffer);
			return(EOF);
		}

		buffer[count] = inChar;

		//printf("inChar: %c\t buffer[%d] = %c\n", inChar, count, buffer[count]);

		if( (count+ 1) > bufferSize){
			#ifdef TESTGETLINE
			fprintf(stderr, "realloc to %d\n", 2*bufferSize);
			#endif
			bufferSize = 2*bufferSize;
			buffer = realloc(buffer, bufferSize);
			if(buffer == NULL){
				fprintf(stderr, "cannot realloc buffer, shit\n");
				exit(1);
			}
		}

		count++;
	} while( inChar != '\n' );
	count--;
	// set the last thing to be "end of string" instead of newline
	buffer[count] = '\0';

	printf("GETLINE(buffer): |%s|\n", buffer);

	// now deal with copying the read line into the supplied 
	// pointer after allocating it
	if(*s)
		free(*s);
	
	// append a \0 to the end, fucking c strings
	*s = malloc(sizeof(char)*(count+1));
	*length = (count+1);
	//s[count+1] = '\0';
	memcpy(*s, buffer, sizeof(char)*(count+1));
	printf("GETLINE(*s): |%s|\n", *s);

	free(buffer);
	return((count+1));
}


//#ifdef TESTGETLINE
//int main (void){
//	int read;
//	size_t length; 
//	char* line = NULL;
//	FILE *fptr;
//	
//	fptr = fopen("test.txt", "r");
//
//	while((read = getline(&line, &length, fptr))!= EOF){
//		printf("Retrieved line of length %d :\n", read);
//	};
//	
//
//	fclose(fptr);
//	free(line);
//	return EXIT_SUCCESS;
//}
//#endif
