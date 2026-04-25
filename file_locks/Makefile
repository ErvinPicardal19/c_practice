
TARGET:=main

C_SOURCES:=$(wildcard *.c)
OBJECTS:=$(C_SOURCES:%.c=%.o)
DEPENDS:=$(OBJECTS:%.o=%.d)

CFLAGS:=-MMD -MP
#LDFLAGS:=-L$(shell pwd)/bank -lbank

all: $(TARGET)

$(TARGET): $(OBJECTS)
	gcc $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

-include $(DEPENDS)

clean:
	rm -rf *.o *.d $(TARGET)

.PHONY: all clean
