CC=gcc
TARGET=archiver

CFLAGS=-Wall -Werror -Wextra

INCLUDE=-I inc

SRC_DIR=src

FILES=archiver.c \
      creator.c

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

check:
	@echo $(SOURCE)
	./checkpatch.pl --no-tree -f $(SOURCE)
