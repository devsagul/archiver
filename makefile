CC=gcc
TARGET=archiver

CFLAGS=-Wall -Werror -Wextra

SRC_DIR=src

FILES= archiver.c \

SOURCE=$(join $(SRC_DIR)/, $(FILES))

OBJ=$(join obj/, $(notdir $(SOURCE:.c=.o)))


all: $(TARGET)
	@true

$(TARGET): $(OBJ)
	@$(CC) $(CFLAGS) -o $@ $^

obj/%.o: src/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(TARGET)

re: fclean all

check:
	./checkpatch.pl --no-tree -f $(SOURCE)
