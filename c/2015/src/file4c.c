#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "file4c.h"
#include "string4c.h"

/**
 * file_read_all_text: Reads all the content of the file into a string.
 * @param result: The resulting string.
 * @param filename: The name of the file to read.
*/
void file_read_all_text(char **result, char *filename) {
	if(filename == NULL) {
		fprintf(stderr, "%s():%d: filename is required\n", __func__, __LINE__);
		return;
	}

	if(access(filename, F_OK) != 0) {
		fprintf(stderr, "%s():%d: file %s does not exist\n", __func__, __LINE__, filename);
		return;
	}

	FILE *file_ptr = fopen(filename, "r");
	
	fseek(file_ptr, 0, SEEK_END);
	long file_size = ftell(file_ptr);
    #ifdef NDEBUG
    printf("%s:%d: Reading %ld bytes from %s\n", __func__, __LINE__, file_size, filename);
    #endif
	fseek(file_ptr, 0, SEEK_SET);
	
	*result = malloc(file_size);
	fread(*result, 1, file_size, file_ptr);

    free(file_ptr);
}

void file_read_all_lines(char ***result, size_t *size, char *filename) {
	char *file_content;

    file_read_all_text(&file_content, filename);
    string_split(result, size, file_content, "\r\n");

    if(result == NULL) {
        fprintf(stderr, "%s:%d: No lines were read\n", __func__, __LINE__);
    }
}
