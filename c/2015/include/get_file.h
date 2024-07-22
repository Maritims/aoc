#ifndef GET_FILE
#define GET_FILE

#include <stdint.h>
#include <stdlib.h>

char *get_file(char* filename);

char **get_file_as_lines(char *filename, size_t *out_size);

char **split_to_lines(char *input, size_t *out_size);

char *get_substring(char *input, uint32_t start, uint32_t length);

#endif
