/*
* Name: Austin Nguyen
* Date: 1/13/2025 (Monday 2:15 PM - 5:00 PM)
* Title: Lab2 – Threads and HTTP
* Description: Lab 2 Part 1: Rewriting the C program from Lab 1 Step 5 using
* multithreading to copy files using function and system calls.
*/


#include <stdio.h>   // fprintf(), fread(), fwrite(), fopen(), fclose()
#include <stdlib.h>  // malloc(), free()
#include <pthread.h> // pthread_create()
#include <unistd.h>  // read(), write()
#include <fcntl.h>   // open(), close()
#include <errno.h>   // errno
#include <time.h>    // clock()

#define BUF_SIZE 2048 //buffer size
#define NUM_FILES 3  // # of files to copy

// data structure to hold copy
struct copy_struct {
	int thread_id; 
	char* src_filename;
	char* dst_filename; 
};

// copies a file from src_filename to dst_filename using functions fopen(), fread(), fwrite(), fclose()
int func_copy(char* src_filename, char* dst_filename) {
	FILE *src, *dst;
	char *buf;
	int b_read;

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
	// reads content of file in loop iterations until end of file
	while ((b_read = fread(buf, 1, BUF_SIZE, src)) > 0) {
		// writes bytes_read to dst_filename 
		fwrite(buf, 1, b_read, dst);
	}

	fclose(src);  // closes src file pointer
	fclose(dst);  // closes dst file pointer
	free(buf);    // frees memory used for buf
	return 0;
}

// copies a file from src_filename to dst_filename using syscalls open(), read(), write(), close()
int syscall_copy(char* src_filename, char* dst_filename)  {
	int src_fd, dst_fd;
	char *buf;
	ssize_t b_read;

	src_fd = open(src_filename, O_RDONLY);	// opens a file for reading
	if (src_fd < 0) { // open() error checking
		fprintf(stderr, "unable to open %s for reading: %i\n", src_filename, errno);
		close(src_fd);
		exit(0);
	}
	dst_fd = open(dst_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);	// open with permissions
	if (dst_fd < 0) { // open() error checking
		fprintf(stderr, "unable to open/create %s for writing: %i\n", dst_filename, errno);
		close(dst_fd);
		exit(0);
	}

	buf = malloc((size_t)BUF_SIZE);  // allocate a buffer to store read data
	while ((b_read = read(src_fd, buf, BUF_SIZE)) > 0) {
		write(dst_fd, buf, b_read);  // writes bytes_read to dst_filename
	}

	close(src_fd);  // closes src_fd file descriptor
	close(dst_fd);  // closes dst_fd file descriptor
	free(buf);      // frees memory used for buf
	return 0;
}

// thread function to copy one file
void* copy_thread(void* arg) {
	struct copy_struct params = *(struct copy_struct*)arg;  // cast/dereference void* to copy_struct
	printf("thread[%i] - copying %s to %s \n", params.thread_id, params.src_filename, params.dst_filename);
	clock_t start_time = clock();  // Start timing func_copy
	func_copy(params.src_filename, params.dst_filename);
	clock_t end_time = clock();    // End timing func_copy
	double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
	// printf("thread[%i] - func_copy completed in %.2f seconds\n", params.thread_id, elapsed_time);
	pthread_exit(NULL);
}

int main() {
	char src_filenames[NUM_FILES][20];
	char dst_filenames[NUM_FILES][20];
	pthread_t threads[NUM_FILES];
	struct copy_struct thread_params[NUM_FILES];

	// Generate file names
	for (int i = 0; i < NUM_FILES; i++) {
		sprintf(src_filenames[i], "src_%d.dat", i + 1);
		sprintf(dst_filenames[i], "dst_%d.dat", i + 1);
	}

	printf("Testing syscall_copy:\n");
	clock_t start_syscall = clock();  // Start timing syscall_copy
	for (int i = 0; i < NUM_FILES; i++) {
		syscall_copy(src_filenames[i], dst_filenames[i]);
	}
	clock_t end_syscall = clock();    // End timing syscall_copy
	double syscall_time = ((double)(end_syscall - start_syscall)) / CLOCKS_PER_SEC;
	printf("syscall_copy completed for all files in %.7f seconds.\n", syscall_time);

	printf("Testing func_copy with multi-threading:\n");
	clock_t start_time = clock();  // Start timing multi-threaded func_copy
	for (int i = 0; i < NUM_FILES; i++) {
		thread_params[i].thread_id = i + 1;
		thread_params[i].src_filename = src_filenames[i];
		thread_params[i].dst_filename = dst_filenames[i];

		if (pthread_create(&threads[i], NULL, copy_thread, (void*)&thread_params[i]) != 0) {
			fprintf(stderr, "Error: Unable to create thread %d\n", i + 1);
			exit(1);
		}
	}

	for (int i = 0; i < NUM_FILES; i++) {
		pthread_join(threads[i], NULL);
	}
	clock_t end_time = clock();  // End timing multi-threaded func_copy
	double thread_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
	printf("Multi-threaded func_copy completed in %.7f seconds.\n", thread_time);

	return 0;
}
