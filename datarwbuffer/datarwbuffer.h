#ifndef datarwbuffer_h
#define datarwbuffer_h

#include <stdlib.h>

typedef struct {
        char *data;
        size_t count;
        size_t capacity;
        char *read_ptr;
        char *write_ptr;
} DataRWBuffer;

/**
 * @brief Read at most `size` bytes from src to dest.
 * 
 * @param src 
 * @param dest 
 * @param size 
 * @return size_t Number of bytes actually read - this may vary if the amount of
 *                bytes left to read in the buffer is less than asked.
 */
size_t read_databuffer (DataRWBuffer *src, void *dest, size_t size);

/**
 * @brief Write `size` bytes to the buffer
 * 
 * @param dest 
 * @param src 
 * @param size 
 * @return size_t 
 */
size_t write_databuffer (DataRWBuffer *dest, void *src, size_t size);

void init_databuffer (DataRWBuffer *buffer);

void destroy_databuffer (DataRWBuffer *buffer);

#endif