CFLAGS = \
  -std=c99 \
  -Wall \
  -Wextra \
  -Wconversion \
  -Wsign-conversion \
  $(DEFINES) \
  $(OPTFLAGS)

DEFINES = \
  -D_POSIX_C_SOURCE=200112L

LDFLAGS = \
  -lm

OBJECTS = \
  src/main.o \
  src/random.o \
  src/simulation.o

PRODUCT = \
  ljbench


.PHONY: all run clean

all: $(PRODUCT)
	@:

run: $(PRODUCT)
	@./$(PRODUCT) -n 500 -t 5000 | awk '{ print $$1 " step/s" }'

clean:
	rm -f $(PRODUCT) $(OBJECTS)

$(PRODUCT): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

src/main.o: src/simulation.h
src/simulation.o: src/simulation.h src/random.h src/vec.h
