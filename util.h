#ifndef UTIL_H
#define UTIL_H

enum
{
   ContinueTimer = true,
   StopTimer = false,
};

#define REINTERPRET(newType, newName, oldName) newType newName = (newType)oldName;

#endif