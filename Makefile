SOURCES = src/devices/audio.c src/devices/controller.c src/devices/datetime.c src/devices/file.c src/devices/mouse.c src/devices/screen.c src/devices/system.c src/uxn.c src/uxnemu.c 

PROGRAM = uxnatr

ifdef CC65_TARGET
CC      = cl65
CFLAGS  = -t $(CC65_TARGET) --create-dep $(<:.c=.d) -O
LDFLAGS = -t $(CC65_TARGET) -m $(PROGRAM).map
else
CC      = gcc
CFLAGS  = -MMD -MP -O
LDFLAGS = -Wl,-Map,$(PROGRAM).map
endif

########################################

.SUFFIXES:
.PHONY: all clean
all: $(PROGRAM)

ifneq ($(MAKECMDGOALS),clean)
-include $(SOURCES:.c=.d)
endif

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(PROGRAM): $(SOURCES:.c=.o)
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	$(RM) $(SOURCES:.c=.o) $(SOURCES:.c=.d) $(PROGRAM) $(PROGRAM).map

