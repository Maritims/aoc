#include <stdint.h>

#define FNV_OFFSET_BASIS 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

uint64_t *fnv1a(const char *bytes);