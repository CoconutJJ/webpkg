#ifndef memory_h
#define memory_h

#include <stdlib.h>

void *allocate (size_t size);
void *reallocate (void *buffer, size_t new_size);

#endif