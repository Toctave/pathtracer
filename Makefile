.RECIPEPREFIX = >

LIBS = sdl2
BINARY_NAME = pathtracer

BINDIR = bin
OBJDIR = obj
SRCDIR = src
SRCS = $(wildcard $(SRCDIR)/*.c) $(wildcard $(SRCDIR)/pcg/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

LDFLAGS = $(shell pkg-config --libs $(LIBS)) -lm -pthread
CFLAGS = $(shell pkg-config --libs $(LIBS)) -pthread -Iinclude -Wall -O3

$(BINDIR)/$(BINARY_NAME) : $(OBJS)
> @ mkdir -p $(BINDIR)
> @ $(CC) $(OBJS) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o : src/%.c
> @ mkdir -p $(OBJDIR) $(OBJDIR)/pcg
> @ $(CC) $(CFLAGS) -c $< -o $@

clean:
> rm -rf $(OBJDIR)
