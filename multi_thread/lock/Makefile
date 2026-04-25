LIBNAME:=lock
LIB:=lib$(LIBNAME).so
LIBSONAME:=$(LIB).1
VERSION:=1

C_SOURCES:=$(wildcard *.c)
OBJECTS:=$(C_SOURCES:%.c=%.o)
DEPENDS:=$(OBJECTS:%.o=%.d)

CFLAGS:=-MMD -MP
LDFLAGS:=-Wl,-soname,$(LIBSONAME)

all: $(LIBSONAME).$(VERSION)

$(LIBSONAME).$(VERSION): $(OBJECTS)
	gcc $(CFLAGS) -shared $(LDFLAGS) $^ -o $@
	ln -sf $@ $(LIBSONAME)
	ln -sf $@ $(LIB)

%.o: %.c
	gcc $(CFLAGS) -fPIC -c $< -o $@

-include $(DEPENDS)

clean:
	rm -rf *.o *.d *.so*

.PHONY: all clean
