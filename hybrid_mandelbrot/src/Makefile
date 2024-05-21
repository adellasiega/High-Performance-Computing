# Compiler
CC = mpicc

# Compiler flags
CFLAGS = -fopenmp -Wall

# Linker flags
LDFLAGS = -lm

# Include directories
INCLUDES = -I.

# Source files
SRCS = main.c mandelbrot.c image_utils.c timing_utils.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
EXEC = mandelbrot

# Build the executable
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJS) $(EXEC)