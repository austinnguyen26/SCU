/* 
Name: Austin Nguyen
Date: 01/17/25
Title: Lab 1 "2.c"
Description: C program to copy files (binary and text) using system calls.
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

// copies a files form src_filename to dst_filename using syscalls open(), read(), write(), close()
int syscall_copy(char* src_filename, char* dst_filename)  {
	//initializing variables of appropriate types again
	int src_fd, dst_fd;
	char *buf;
	ssize_t b_read;

	src_fd = open(src_filename, O_RDONLY);	// opens a file for reading
	if (src_fd < 0) { // open() error checking
		fprintf(stderr, "unable to open %s for reading: %i\n", src_filename, errno);
		close(src_fd);
		exit(0);
	}
	// O_WRONLY - write only
	// O_CREAT - create file if it doesn't exist
	// O_TRUNC - truncates file to length 0 if it already exists (deletes existing file)
	// opens a file for writing; erases old file/creates a new file
	dst_fd = open(dst_filename, O_WRONLY | O_CREAT | O_TRUNC);	
	if (dst_fd < 0) { // open() error checking
		fprintf(stderr, "unable to open/create %s for writing: %i\n", dst_filename, errno);
		close(dst_fd);
		exit(0);
	}


	// read/ write loop
	buf = malloc((size_t)BUF_SIZE);   // allocate a buffer to store read data
	// reads up to BUF_SIZE bytes from src_filename
	while((b_read = read(src_fd, buf, BUF_SIZE)) > 0){
		// writes bytes_read to dst_filename
		write(dst_fd, buf, b_read);
	}
	
	// closes src_fd file descriptor
	close(src_fd);
	// closes dst_fd file descriptor
	close(dst_fd);
	// frees memory used for buf
	free(buf);

	return 0;
}

int main(int argc, char *argv[]){
	// if (argc != 3){}

	printf(argv[1]);
	printf("\n");
	printf(argv[2]);
	printf("\n");
	syscall_copy(argv[1], argv[2]);
	return 0;
}