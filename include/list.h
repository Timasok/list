#ifndef LIST_H
#define LIST_H

#include <string.h>

//DO NOT USE REALLOC YET
const char VALUE_POISON = ' ';
const char NIL = '#';
const char OPERATION_INFO_LEN = 128;
const int LIST_RESIZE_TO_STRETCH = 2;
const int LIST_RESIZE_TO_SHRINK = 4;

typedef char elem_d;

struct List_elem
{
    elem_d value;
    int next;
    int prev;
};

struct List
{

    size_t free_index;
    size_t size;
    size_t capacity;
    int code_of_error;
    List_elem * elements;
    List_elem * free;
    char operation_info[128] = {};

};

#define LIST_CTOR(list, capacity)                                                              \
        listCtor(&list, capacity, #capacity, #list, __FILE__, __FUNCTION__,__LINE__)


int listCtor(List *list, size_t capacity, const char * string_capacity, const char * name_of_var, 
                                    const char * name_of_file, const char * name_of_func, int number_of_line);

#define listDelAfter(list, index) listDelAfter_(list, index, #index)
#define listDelBefore(list, index) listDelBefore_(list, index, #index)

int listDelAfter_(List *list, int index, const char * args);
int listDelBefore_(List *list, int index, const char * args);

int getTail(List *list);
int getHead(List *list);

[[nodiscard]] int findElemInOrder(List *list, int logical_number);

int listSort(List *list);

int listResize(List *list, size_t new_capacity);

#define listAddAfter(list, value, index) listAddAfter_(list, value, index, #value, #index)
#define listAddBefore(list, value, index) listAddAfter_(list, value, index, #value, #index)

int listAddAfter_(List *list, elem_d value, int index, const char *arg_value, const char * arg_index);
int listAddBefore_(List *list, elem_d value, int index, const char *arg_value, const char * arg_index);

int listDtor(List *list);

[[nodiscard]] int getSizeOfFree(List *list);
//index of element after which we are going to paste new element
int addElement(List *list, elem_d value, int index);
//index of element which will be deletedint getNextFree(List *list)
int delElement(List *list, int index);

int getNextFree(List *list);
int fillWithPoison(List *list, int beginning, int end);

#endif