#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "file4c.h"
#include "string4c.h"

static bool file_is_invalid(char *filename) {
	if(filename == NULL) {
		fprintf(stderr, "%s():%d: filename is required\n", __func__, __LINE__);
		return true;
	}

	if(access(filename, F_OK) != 0) {
		fprintf(stderr, "%s():%d: file %s does not exist\n", __func__, __LINE__, filename);
		return true;
	}

    return false;
}

char *file_read_all_text(char *filename) {
    if(file_is_invalid(filename)) {
        return NULL;
    }

	FILE *file = fopen(filename, "r");	
	fseek(file, 0, SEEK_END);
	long file_size = ftell(file) + 1;
	fseek(file, 0, SEEK_SET);
	
	char *result = malloc(file_size);
	int bytes_read = fread(result, 1, file_size, file);
    result[bytes_read] = '\0';

    fclose(file);
    return result;
}

char **file_read_all_lines(size_t *out_number_of_lines, char *filename) {
    if(file_is_invalid(filename)) {
        return NULL;
    }

	char *file_content = file_read_all_text(filename);
    size_t number_of_lines = 0;
    char **lines = string_split(&number_of_lines, file_content, "\r\n");
    free(file_content);

    *out_number_of_lines = number_of_lines;
    return lines;
}
