
TARGET:=main

C_SOURCES:=$(wildcard *.c)
OBJECTS:=$(C_SOURCES:%.c=%.o)
DEPENDS:=$(OBJECTS:%.o=%.d)

CFLAGS:=-MMD -MP

all: $(TARGET)

$(TARGET): $(OBJECTS)
	gcc $^ -o $@ $(LDFLAGS)

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o *.d $(TARGET)

-include $(DEPENDS)

.PHONY: all clean
