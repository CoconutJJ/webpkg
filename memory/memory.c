#include <stdio.h>
#include <stdlib.h>

void *allocate (size_t size)
{
        void *buffer = malloc (size);

        if (!buffer) {
                perror ("malloc");
                exit (EXIT_FAILURE);
        }

        return buffer;
}
void *reallocate (void *buffer, size_t new_size)
{
        buffer = realloc (buffer, new_size);

        if (!buffer) {
                perror ("realloc");
                exit (EXIT_FAILURE);
        }
        return buffer;
}