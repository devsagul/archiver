CC=gcc
TARGET=archiver

CFLAGS=-Wall -Werror -Wextra

INCLUDE=-I inc

SRC_DIR=src

FILES=archiver.c	\
      creator.c		\
      tree.c		\
      smartstr.c	\
      printer.c		\
      extractor.c	\

SOURCE=$(patsubst %.c, src/%.c, $(FILES))

OBJ=$(patsubst src/%.c, obj/%.o, $(SOURCE))

all: $(TARGET)
	@true

$(TARGET): $(OBJ)
	@$(CC) $(CFLAGS) -o $@ $^

obj/%.o: src/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(TARGET)

re: fclean all

check_formatting:
	@echo $(SOURCE)
	./checkpatch.pl --no-tree --terse --show-types --strict --ignore CONST_STRUCT -f $(SOURCE)

check: check_formatting

test: re check
	@$(CC) tests/tree.c src/tree.c src/smartstr.c -I inc -o tests/tree
	./tests/tree
	rm -f tests/tree
	cd tests; sh main_test.sh
