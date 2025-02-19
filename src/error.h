#ifndef ERROR_H
#define ERROR_H

#include <stdlib.h>

#define critical_error(format, ...) {\
    printf(format "  %s %d", __VA_ARGS__, __FILE__, __LINE__); \
    exit(-1);}

// Used for errors that should abort the program, but printing file/lines are not useful.
// For end users
#define critical_error_no_line_print(format, ...) {\
    printf("Aborting: " format, __VA_ARGS__); \
    exit(-1);}

#endif