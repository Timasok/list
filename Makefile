I_FLAG := -I./include/ -I./inc/

CC:= g++
CFLAGS := -g -lm $(I_FLAG) #-Wall -fsanitize=address

SRC_DIR := ./src/

SOURCES 	:= $(SRC_DIR)main_list.cpp $(SRC_DIR)funcs_list.cpp $(SRC_DIR)list_debug.cpp 
EXECUTABLE  := list

all:
	$(CC) $(SOURCES) $(CFLAGS) -o $(EXECUTABLE)