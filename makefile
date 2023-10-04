EXCUTABLE=server
SOURCES=$(wildcard src/*.c)
OBJECTS=$(SOURCES:.c=.o)
DEPS=$(SOURCES:.c=.d)
FLAGS=-Wall -g


all: $(EXCUTABLE)

-include $(DEPS)

src/%.o: src/%.c
	gcc -c $(FLAGS) -MMD $< -o $@

$(EXCUTABLE): $(OBJECTS)
	$(CC) $(FLAGS) $^ -o $@

clean:
	rm -f $(OBJECTS) $(EXCUTABLE)

.PNONY: all clean