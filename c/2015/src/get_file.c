#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "get_file.h"
#include "string4c.h"

char *get_file(char *filename) {
	if(filename == NULL) {
		fprintf(stderr, "filename is required\n");
		return NULL;
	}

	if(access(filename, F_OK) != 0) {
		fprintf(stderr, "File %s does not exist\n", filename);
		return NULL;
	}

	FILE *fp = fopen(filename, "r");
	
	fseek(fp, 0, SEEK_END);
	long fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	char *fileContent = malloc(fileSize);
	fread(fileContent, 1, fileSize, fp);

	return fileContent;
}

char **get_file_as_lines(char *filename, size_t *out_size) {
	char *file_content = get_file(filename);
	size_t number_of_lines;
	char **lines = string_split(file_content, "\r\n", &number_of_lines);
	//char **lines = split_to_lines(file_content, &number_of_lines);

	*out_size = number_of_lines;
	return lines;
}

char *get_substring(char *input, uint32_t start, uint32_t length)
{
    if (start > strlen(input))
    {
        fprintf(stderr, "The starting position %d exceeds the length %d of the input string \"%s\"\n", start, length, input);
        return NULL;
    }

    char *output = malloc(length + 1);
    if (output == NULL)
    {
        fprintf(stderr, "Unable to allocate memory for output\n");
        return NULL;
    }

    strncpy(output, input + start, length);
    output[length] = '\0';

    return output;
}