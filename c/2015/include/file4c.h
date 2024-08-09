#ifndef FILE4C
#define FILE4C

#include <stdint.h>
#include <stdlib.h>

char *file_read_all_text(char* filename);

char **file_read_all_lines(size_t *out_number_of_lines, char *filename);

#endif
