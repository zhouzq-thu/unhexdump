PROGRAM := unhexdump
TEST_FILEPATH := /usr/bin/man
TEST_FILE := $(shell basename $(TEST_FILEPATH))

SRCS = unhexdump.c

$(PROGRAM): $(SRCS)
	gcc -O2 -o $@ $^

test:
	hexdump -C $(TEST_FILEPATH) > $(TEST_FILE).dump
	./unhexdump -i $(TEST_FILE).dump -o $(TEST_FILE)
	chmod +x $(TEST_FILE)
	./$(TEST_FILE)

all: $(PROGRAM)

.PHONY: clean
clean:
	rm -rf $(PROGRAM) $(TEST_FILE).dump $(TEST_FILE)
