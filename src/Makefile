# Compiler
CC = g++

# Compile time flags
CFLAGS = -Wall -g

# Define the executable file
TARGET = main

$(TARGET): main.o
				$(CC) $(CFLAGS) -o $(TARGET) main.o -lglfw -lGLEW -lGL

main.o: main.cpp
				$(CC) $(CFLAGS) -c main.cpp

clean:
				$(RM) $(TARGET) *.o *~
