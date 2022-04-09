#include "../memory/memory.h"
#include <stdlib.h>
void *resize_array (void *array, size_t membsize, size_t nmemb, size_t *membcapacity)
{
        size_t capacity = *membcapacity;

        if (capacity == 0)
                capacity = 1;
        else if (nmemb == capacity)
                capacity *= 2;
        else if (nmemb < capacity / 4)
                capacity /= 2;

        array = reallocate (array, capacity * membsize);

        *membcapacity = capacity;

        return array;
}
