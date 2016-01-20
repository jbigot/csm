#ifndef __MYTIMEISDEF__
#define __MYTIMEISDEF__

#include <time.h>
#include <sys/time.h>
typedef struct timeval my_time_t;
#define get_time(a) gettimeofday(&a, NULL)
#define timing_diff_usec(a,b) (((long long)(b).tv_sec - (a).tv_sec)*1000000+((long long)(b).tv_usec - (a).tv_usec))

#endif
