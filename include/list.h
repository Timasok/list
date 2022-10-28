#ifndef LIST_H
#define LIST_H

#include "stack_funcs.h"

//DO NOT USE REALLOC YET
const int LIST_SIZE = 10;
const char VALUE_POISON = ' ';
const char NIL = '#';

typedef char elem_d;

struct List_elem
{
    elem_d value;
    int next;
    int prev;
};

struct List
{
    size_t head;
    size_t tail;
    size_t free_index;
    size_t size;
    size_t capacity;
    int code_of_error;
    List_elem * elements;
    Stack free;

};

#define LIST_CTOR(list)                                                              \
        listCtor(&list, #list, __FILE__, __FUNCTION__,__LINE__)

int listCtor(List *list, const char * name_of_var, 
                                    const char * name_of_file, const char * name_of_func, int number_of_line);
//index of element after which we are going to paste new element
int addElement(List *list, elem_d value, int index);
//index of element which will be deleted
int delElement(List *list, int index);
int listDtor(List *list);
[[nodiscard]] int findElemInOrder(List *list, int logical_number);
int listSort(List *list);
int listResize(List *list);
int fillWithPoison(List *list, int beginning, int end);

#endif