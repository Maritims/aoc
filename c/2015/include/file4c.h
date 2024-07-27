#ifndef FILE4C
#define FILE4C

#include <stdint.h>
#include <stdlib.h>

void file_read_all_text(char **result, char* filename);

void file_read_all_lines(char ***result, size_t *size, char *filename);

#endif
