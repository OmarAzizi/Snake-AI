CC = gcc
CFLAGS = -Wall -Wextra
INCLUDES = -I/usr/include/SDL2
LIBS = -lSDL2
SOURCE = snake.c
OBJECTS = $(SOURCE:.c=.o)
MAIN = snake

build: $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJECTS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) *.o *~ $(MAIN)
