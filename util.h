#ifndef UTIL_H
#define UTIL_H

enum
{
   ContinueTimer = true,
   StopTimer = false,
};

#define REINTERPRET(newType, newName, oldName) newType newName = (newType)oldName;

#define IGNORE(arg) (void)arg;

#define MIN(a, b) (a) < (b) ? (a) : (b)

#define MAX(a, b) (a) > (b) ? (a) : (b)

#endif