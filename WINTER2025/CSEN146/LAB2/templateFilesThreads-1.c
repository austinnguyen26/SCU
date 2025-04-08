// COEN 146L : Lab 2 - template to use for creating multiple thread to make file transfers (step 1)

#include <stdio.h>   // fprintf(), fread(), fwrite(), fopen(), fclose()
#include <stdlib.h>  // malloc(), free()
#include <pthread.h> // pthread_create()
#include <unistd.h>  // read(), write()
#include <fcntl.h>   // open(), close()
#include <errno.h>   // errno
#include <time.h>    // clock()

#define BUF_SIZE 2048 //buffer size
#define NUM_FILES 10  // # of files to copy

// data structure to hold copy
struct copy_struct {
	int thread_id; 
	char* src_filename;
	char* dst_filename; 
};

// copies a files from src_filename to dst_filename using functions fopen(), fread(), fwrite(), fclose()
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
	// reads content of file is loop iterations until end of file
		while((b_read = fread(buf, 1, BUF_SIZE, src)) > 0){
		// writes bytes_read to dst_filename 
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

// thread function to copy one file
void* copy_thread(void* arg) {
	struct copy_struct params = *(struct copy_struct*)arg;  // cast/dereference void* to copy_struct
	printf("thread[%i] - copying %s to %s\n", params.thread_id, params.src_filename, params.dst_filename);
	//call file copy function
	func_copy(params.src_filename, params.dst_filename);
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

	clock_t start_time = clock(); 

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

	clock_t end_time = clock(); 
	double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
	printf("Multi-threaded file copying completed in %.2f seconds.\n", elapsed_time);

	return 0;
}