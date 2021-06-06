src = $(wildcard src/*.cpp)
obj = $(src:.cpp=.o)
dep = $(obj:.o=.d)

LDFLAGS = -lglfw -lGLEW -lGL
CC = g++
CFLAGS = -Wall -g -MMD

TARGET = main

$(TARGET): $(obj)
		$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

-include $(dep)

.PHONY: clean
clean:
		$(RM) $(TARGET) $(obj)

.PHONY: cleandep
cleandep:
		$(RM) $(dep)
