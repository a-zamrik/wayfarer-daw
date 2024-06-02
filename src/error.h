#ifndef ERROR_H
#define ERROR_H

#include <stdlib.h>

#define critical_error(format, ...) {\
    printf(format " %s %d", __VA_ARGS__, __FILE__, __LINE__); \
    exit(-1);}

#endif