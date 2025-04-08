/* 
Name: Austin Nguyen
Date: 01/17/25
Title: Lab 1 "1.c"
Description: C program to copy files (binary and text) using functions.
*/

#include <stdio.h>   // fprintf(), fread(), fwrite(), fopen(), fclose()
#include <stdlib.h>  // malloc(), free()
#include <pthread.h> // pthread_create()
#include <unistd.h>  // read(), write()
#include <fcntl.h>   // open(), close()
#include <errno.h>   // errno
#include <time.h>    // clock()
#include <sys/mman.h> // mmap()

#define BUF_SIZE 2048 //buffer size

// copies a files from src_filename to dst_filename using functions fopen(), fread(), fwrite(), fclose()
int func_copy(char* src_filename, char* dst_filename) {
	//initialize variables used in template as appropriate types
	FILE *src, *dst;
	char *buf;
	int b_read;	//bytes read

	src = fopen(src_filename, "r");	// opens a file for reading
	if (src == NULL) { // fopen() error checking
		fprintf(stderr, "unable to open %s for reading: %i\n", src_filename, errno);
		exit(0);
	}
	dst = fopen(dst_filename, "w");	// opens a file for writing; erases old file/creates a new file
	if (dst == NULL) { // fopen() error checking
		fprintf(stderr, "unable to open/create %s for writing: %i\n", dst_filename, errno);
		exit(0);
	}

	buf = malloc((size_t)BUF_SIZE);  // allocate a buffer to store read data
	// reads content of file is loop iterations until end of file
	
	// writes bytes_read to dst_filename 
	while((b_read = fread(buf, 1, BUF_SIZE, src)) > 0){
		fwrite(buf, 1, b_read, dst);
	}
	// closes src file pointer
	fclose(src);
	// closes dst file pointer
	fclose(dst);
	// frees memory used for buf
	free(buf);
	return 0;
}

int main(int argc, char *argv[]){

	printf(argv[1]);
	printf("\n");
	printf(argv[2]);
	printf("\n");
	func_copy(argv[1], argv[2]);
	return 0;
}