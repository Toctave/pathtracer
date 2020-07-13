#pragma once

#include <stdio.h>
#include <signal.h>
#include <math.h>

#ifdef DEBUG
#define ASSERT(x) \
    if (!(x)) \
    { \
	fprintf(stderr, "Assertion failed : %s (%s:%d)\n", #x, __FILE__, __LINE__); \
	raise(SIGABRT); \
    }

#define FEQ(x, y) (fabs((x) - (y)) < 1e-2)

#else

#define ASSERT(x)
#define FEQ(x, y)

#endif
