#include <time.h>
#include <sys/time.h>

#include "ETime.h"

double GetTime()
{
    clock_t current_time = clock();
    return current_time;
}
