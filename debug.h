#pragma once

#include <stdio.h>
#include <signal.h>
#include <math.h>

#define ASSERT(x) \
    if (!(x)) \
    { \
	fprintf(stderr, "Assertion failed : %s (%s:%d)\n", #x, __FILE__, __LINE__); \
	raise(SIGABRT); \
    }

#define FEQ(x, y) (fabs((x) - (y)) < 1e-3)
