#ifndef array_h
#define array_h
#include <stdlib.h>
#define ARRAY_UPDATE_SIZE(arrayPtr, elemType, memberCount, memberCapacityPtr)                                          \
        do {                                                                                                           \
                (arrayPtr) = resize_array (arrayPtr, sizeof (elemType), memberCount, memberCapacityPtr);               \
        } while (0)

void *resize_array (void *array, size_t membsize, size_t nmemb, size_t *membcapacity);
#endif