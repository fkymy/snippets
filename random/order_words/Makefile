OUT = order_words
SRCS_PATH = .
INCLUDES_PATH = .
CC = gcc
CCFLAGS = -g -Wall -Werror -Wextra -fsanitize=address -fsanitize=undefined
OPTIONS = -I $(INCLUDES_PATH)
OBJECTS = $(SRCS_PATH)/order_words.o $(SRCS_PATH)/str_to_dict.o $(SRCS_PATH)/compare_dicts.o

all: $(OUT)
$(OUT) :
	$(CC) $(CCFLAGS) $(OPTIONS) -c $(SRCS_PATH)/compare_dicts.c -o $(SRCS_PATH)/compare_dicts.o
	$(CC) $(CCFLAGS) $(OPTIONS) -c $(SRCS_PATH)/str_to_dict.c -o $(SRCS_PATH)/str_to_dict.o
	$(CC) $(CCFLAGS) $(OPTIONS) -c $(SRCS_PATH)/order_words.c -o $(SRCS_PATH)/order_words.o
	$(CC) $(CCFLAGS) $(OPTIONS) $(OBJECTS) -o order_words

.PHONY: clean
clean:
	rm -f $(OBJECTS)

.PHONY: fclean
fclean:
	rm -f $(OUT)

.PHONY: re
re: fclean all
