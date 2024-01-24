#include "brainfck.h"

void freesb(struct sbuilder* sb) {
	// free the string pointer before freeing the string builder
	if (sb != NULL) {
		free(sb->sp);
		free(sb);
	}
}

int freadfull(char *fname, struct sbuilder** sb) {
	FILE *fp;

	// open the file in 'read'
	fp = fopen(fname, "r");
	if (fp == NULL) {
		printf("Failed to open file - errno: %d\n", errno);
		return errno;
	}

	// go to the end of the file to get the size
	fseek(fp, 0, SEEK_END);
	size_t fsize = ftell(fp);

	// go back to the start of the file
	rewind(fp);

	// allocate memory to the string builder 'sb'
	// and the string pointer 'sp'
	*sb = malloc(1 * sizeof(struct sbuilder));
	(*sb)->sp = malloc(fsize * sizeof(char));

	// read the file and confirm that the number of bytes read
	// matches the expectation
	size_t read = fread((*sb)->sp, 1, fsize, fp);
	if (read != fsize) {
		int err = ferror(fp);
		printf("Failed to fully read the file - errno: %d\n", err);
		
		// closing the file here in case something has gone wrong with the read
		fclose(fp);
								
		return err;
	}

	// close the file
	fclose(fp);
	
	return 0;
}

