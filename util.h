#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>

enum
{
   ContinueTimer = true,
   StopTimer = false,
};

#define REINTERPRET(newType, newName, oldName) newType newName = (newType)oldName;

#define IGNORE(arg) (void)arg;

#ifndef MIN
#define MIN(a, b) (a) < (b) ? (a) : (b)
#endif

#ifndef MAX
#define MAX(a, b) (a) > (b) ? (a) : (b)
#endif

#endif