#ifndef __CONSTANTS_H_INCLUDED__
#define __CONSTANTS_H_INCLUDED__

typedef int bool;
typedef unsigned char uint8_t;

typedef enum state
{
    _free,
    _taken
}state;

#define true 1
#define false 0

#define Ko 1024
#define Mo 1024 * 1024
#define Go 1024 * 1024 * 1024

#define Kb 1000
#define Mb 1000 * 1000
#define Gb 1000 * 1000 * 1000

#endif // __CONSTANTS_H_INCLUDED__