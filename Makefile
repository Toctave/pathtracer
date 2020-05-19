.RECIPEPREFIX = >

LIBS = sdl2
BINARY_NAME = raytracer
SRCS = $(wildcard *.c)
LDFLAGS = $(shell pkg-config --libs $(LIBS)) -lm
CFLAGS = $(shell pkg-config --libs $(LIBS)) -g

$(BINARY_NAME) : $(SRCS)
> @ $(CC) $(CFLAGS) $(SRCS) -o $@ $(LDFLAGS)

run : $(BINARY_NAME)
> ./$(BINARY_NAME)
