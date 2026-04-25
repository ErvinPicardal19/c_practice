
TARGET:=main

C_SOURCES:=$(wildcard *.c)
OBJECTS:=$(C_SOURCES:%.c=%.o)

CFLAGS:=-Wall -Wextra -Wpedantic -O2
#CFLAGS+= -Ilibs/uthash/include

all: $(TARGET)

$(TARGET): $(OBJECTS)
	gcc -o $@ $^

%.o: %.c
	gcc $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *.o $(TARGET)

.PHONY: all clean
