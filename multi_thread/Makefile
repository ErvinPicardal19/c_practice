
TARGET:=main

C_SOURCES:=$(wildcard *.c)
OBJECTS:=$(C_SOURCES:%.c=%.o)
DEPENDS:=$(OBJECTS:%.o=%.d)

SUBDIR+=bank
SUBDIR+=lock

CFLAGS:=-MMD -MP
CFLAGS+= -Ibank -Ilock

LDFLAGS:=-L$(shell pwd)/bank -lbank

all: $(SUBDIR)/ $(TARGET)

$(TARGET): $(OBJECTS)
	gcc $^ -o $@ $(LDFLAGS)

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

$(SUBDIR)/:
	$(MAKE) -C $@ $(MAKECMDGOALS)

clean: $(SUBDIR)/
	rm -rf *.o *.d $(TARGET)

-include $(DEPENDS)

.PHONY: all clean $(SUBDIR)/
