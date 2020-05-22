.RECIPEPREFIX = >

LIBS = sdl2
BINARY_NAME = raytracer
SRCS = $(wildcard *.c)
LDFLAGS = $(shell pkg-config --libs $(LIBS)) -lm -pthread
CFLAGS = $(shell pkg-config --libs $(LIBS)) -pthread

DEBUG_FLAGS = -g -pg
RELEASE_FLAGS = -O3

$(BINARY_NAME) : $(SRCS)
> @ $(CC) $(CFLAGS) $(RELEASE_FLAGS) $(SRCS) -o $@ $(LDFLAGS)

debug : $(SRCS)
> @ $(CC) $(CFLAGS) $(DEBUG_FLAGS) $(SRCS) -o $(BINARY_NAME) $(LDFLAGS)

run : $(BINARY_NAME)
> ./$(BINARY_NAME)
