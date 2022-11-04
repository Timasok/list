I_FLAG := -I./include/ -I./lib/stack/include/ -I./inc/

CC:= g++
CFLAGS := -g -lm $(I_FLAG) #-Wall -fsanitize=address

SRC_DIR := ./src/
LIB_DIR := ./lib/
SRC_STK := ./lib/stack/src/

SOURCES 	:= $(SRC_DIR)main_list.cpp $(SRC_DIR)funcs_list.cpp $(SRC_DIR)list_debug.cpp $(SRC_STK)stack_funcs.cpp $(SRC_STK)debug_funcs.cpp 
EXECUTABLE  := list

all:
	$(CC) $(SOURCES) $(CFLAGS) -o $(EXECUTABLE)