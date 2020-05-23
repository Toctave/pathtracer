.RECIPEPREFIX = >

LIBS = sdl2
BINARY_NAME = raytracer

OBJDIR = obj
SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c, $(OBJDIR)/%.o, $(SRCS))

LDFLAGS = $(shell pkg-config --libs $(LIBS)) -lm -pthread
CFLAGS = $(shell pkg-config --libs $(LIBS)) -pthread -Iinclude -O3

$(BINARY_NAME) : $(OBJS)
> @ $(CC) $(OBJS) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o : %.c
> @ mkdir -p $(OBJDIR)
> @ $(CC) $(CFLAGS) -c $< -o $@

run : $(BINARY_NAME)
> ./$(BINARY_NAME)
