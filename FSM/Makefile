
TARGET:=main
PLATFORM=3

C_SOURCES:= $(wildcard *.c)
C_SOURCES+= $(wildcard FSM/*.c)
C_SOURCES+= $(wildcard sensor/*.c)

OBJECTS:=$(C_SOURCES:%.c=%.o)
DEPENDS:=$(OBJECTS:%.o=%.d)

CFLAGS:=-MMD -MP -Iinclude/

all: $(TARGET)

$(TARGET): $(OBJECTS)
	gcc $^ -o $@ $(LDFLAGS)

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJECTS) $(DEPENDS) $(TARGET)

-include $(DEPENDS)

.PHONY: all clean
