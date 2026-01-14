CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lgpiod
TARGET = clignoteur

all: $(TARGET)

$(TARGET): clignoteur.c
	$(CC) $(CFLAGS) -o $(TARGET) clignoteur.c $(LIBS)

clean:
	rm -f $(TARGET)
