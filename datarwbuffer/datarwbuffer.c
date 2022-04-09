#include "datarwbuffer.h"
#include "../memory/memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_databuffer (DataRWBuffer *buffer)
{
        buffer->capacity = 128;
        buffer->count = 0;
        buffer->data = allocate (buffer->capacity);
        buffer->read_ptr = buffer->data;
        buffer->write_ptr = buffer->data;
}

void destroy_databuffer (DataRWBuffer *buffer)
{
        free (buffer->data);
}

size_t read_databuffer (DataRWBuffer *src, void *dest, size_t size)
{
        // read nothing if at end of buffer
        if (src->read_ptr == src->data + src->count)
                return 0;

        // data left to be read
        size_t left = src->data + src->count - src->read_ptr;

        // clamp the total read bytes to the amount left in buffer
        size_t total_read = size < left ? size : left;

        memcpy (dest, src->data, total_read);

        src->read_ptr += total_read;

        return total_read;
}

size_t write_databuffer (DataRWBuffer *dest, void *src, size_t size)
{
        while (dest->capacity - dest->count < size) {
                dest->capacity *= 2;
                dest->data = reallocate (dest->data, dest->capacity);
        }
        memcpy (dest->write_ptr, src, size);
        dest->write_ptr += size;
        dest->count = dest->write_ptr - dest->data;
        return size;
}
