#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "list_debug.h"

#define DBG_OUT fprintf(stderr, "file: %s func: %s line: %d\n", __FILE__, __FUNCTION__, __LINE__)

#define PRINT_ERR(...)                                                  \
        do {                                                            \
            fprintf(stderr,"\e[0;31mERROR: " );                         \
            fprintf(stderr, __VA_ARGS__);                               \
            fprintf(stderr,"\e[0m\n" );                                 \
            }while(0)

static int getInitialSize(size_t capacity)
{
    return capacity*0.7;
}

int listCtor(List *list, size_t capacity, const char * string_capacity, const char * name_of_var, 
                                    const char * name_of_file, const char * name_of_func, int number_of_line)
{   
    PARSE_ERROR(list, list == NULL, LIST_ERROR_LIST_IS_NULL);
    PARSE_ERROR(list, capacity < 0, LIST_ERROR_CAPACITY_BELOW_ZERO);

    list->capacity = capacity;
    int initial_size = getInitialSize(capacity);

    list->size = initial_size;

    list->elements = (List_elem *)calloc(list->capacity, sizeof(List_elem));

    for (int counter = 1; counter < initial_size-1; counter++)
    {
        list->elements[counter] = {.value = (elem_d)(64 + counter), .next = counter + 1, .prev = counter - 1};

    }

    list->elements[initial_size-1] = {.value = (elem_d)(64 + initial_size), .next = 0, .prev = initial_size - 2};

    list->elements[0] = {.value = '#', .next = 1, .prev = initial_size - 1};

    // stackCtor(list->free, list->capacity - list->elements[0].prev);
    // list->free = (List_elem *)calloc(list->capacity - getTail(list), sizeof(List_elem));
    fillWithPoison(list, initial_size, list->capacity);
        
    list->free_index = initial_size;

    list->code_of_error = 0;

    sprintf(list->operation_info, "func %s of %s capacity %s", __FUNCTION__, name_of_var, string_capacity);



    LIST_DUMP(list);

    return 1;
}

int fillWithPoison(List *list, int beginning, int end)
{
    for (int counter = beginning; counter < end; counter++)
    {
        list->elements[counter] = {.value =  VALUE_POISON, .next = -1, .prev = - 1};
        // stackPush(&list->free, counter);

    }

    return 1;
}

static int getFree(List *list, int *firstEmpty)
{
    int counter = *firstEmpty;
    for(; list->elements[counter].next != -1 && counter < list->capacity; counter++)
        ;
    
    *firstEmpty = counter;

    // stackPop(&list->free, firstEmpty);

    return 1;
}

static int fillWithPoison(List *list, int index)
{
    list->elements[index] = {.value = ' ', .next = -1, .prev = -1};
    
    // stackPush(&list->free, index);

    return 1;

}

static int swapInNext(List *list, int index, int tmp_next)
{
    list->elements[tmp_next].next = list->elements[index].next;
    list->elements[index].next = tmp_next;

    return 1;
}

static int swapInPrev(List *list, int tmp_prev, int tmp_next)
{
    list->elements[tmp_next].prev = list->elements[tmp_prev].prev;
    list->elements[tmp_prev].prev = tmp_next;

    return 1;
}

static int swapToDel(List *list, int index)
{
    int tmp = 0;

    tmp = list->elements[index].prev;
    list->elements[tmp].next = list->elements[index].next;

    tmp = list->elements[index].next;
    list->elements[tmp].prev = list->elements[index].prev;

    return 1;
}

static int checkForTail(List *list, int index)
{

    if (index == (getTail(list) - 1))
    {
        list->elements[0].prev--;
        return 1;
    } 

    if( index >= getTail(list))
    {
        PARSE_ERROR(list, index >= getTail(list), LIST_ERROR_ATTEMPT_TO_DELETE_AFTER_TAIL);
        return LIST_ERROR_ATTEMPT_TO_DELETE_AFTER_TAIL;

    }

    return 1;   
}

static int checkForHead(List *list, int index)
{
    if (index == getHead(list))
    {
        list->elements[0].next++;
        return 1;
    } 

    if (index < getHead(list))
    {
        PARSE_ERROR(list, index >= getTail(list), LIST_ERROR_ATTEMPT_TO_DELETE_BEFORE_HEAD);
        return LIST_ERROR_ATTEMPT_TO_DELETE_BEFORE_HEAD;

    }

    return 1;    
}

int delElement(List *list, int index)
{
    checkForHead(list, index);
    checkForTail(list, index);

    swapToDel(list, index);
    fillWithPoison(list, index);

    return 1;
}

int addElement(List *list, elem_d value, int index)
{

    list->elements[0].prev++;

    int tmp_next = 1;

    getFree(list, &tmp_next);

    if (tmp_next < index)
    {
        PARSE_ERROR(list, tmp_next < index, LIST_ERROR_INVALID_INPUT_INDEX);
        return LIST_ERROR_INVALID_INPUT_INDEX;
    }

    list->elements[tmp_next].value = value;
    
    int tmp_prev = 1;

    getFree(list, &tmp_prev);

    // printf("tmp_next = %d tmp_prev = %d\n", tmp_next, tmp_prev);

    swapInNext(list, index, tmp_next);
    swapInPrev(list, tmp_prev, tmp_next);

    return 1;
}

int getHead(List *list)
{
    return list->elements[0].next;
}

int getTail(List *list)
{
    return list->elements[0].prev;
}

int getNextFree(List *list)
{
    for (int idx = 0; idx < list->capacity; idx++)
    {
        if (idx != list->free_index && list->elements[idx].next == -1)
        {
            list->free_index = idx;
            return list->free_index; 
        }
    }

    listResize(list, list->capacity * LIST_RESIZE_TO_STRETCH);

    return -1;

}

int listAddAfter_(List *list, elem_d value, int index, const char *arg_value, const char * arg_index)
{
    if (list->elements[index].next == -1)
    {
        list->code_of_error |= LIST_ERROR_INCORRECT_OPERATION;
        return LIST_ERROR_INCORRECT_OPERATION;

    }
    int tmp_free = list->free_index;
    int tmp_next = list->elements[index].next;

    //change elem[index]
    list->elements[index].next = tmp_free; 

    //change elem[tmp_free]
    list->elements[tmp_free].next = tmp_next;
    list->elements[tmp_free].prev = index;
    list->elements[tmp_free].value = value;


    //change elem[tmp_next]
    list->elements[tmp_next].prev = tmp_free;
    
    // list->elements[0].next = tmp_free;

    getNextFree(list);

    sprintf(list->operation_info, "func %s value %s index %s", __FUNCTION__, arg_value, arg_index);

    LIST_DUMP(list);

    return tmp_free;

}

int listAddBefore_(List *list, elem_d value, int index, const char *arg_value, const char * arg_index)
{
    char argument[64] = {};
    sprintf(argument, "prev[%s]", arg_index);
    int tmp = listAddAfter_(list, value, list->elements[index].prev, arg_value, argument);

    return tmp;
}

[[nodiscard]] int getSizeOfFree(List *list)
{
    int num = 0;
    for (int i = 0; i < list->capacity; i++)
    {
        if (list->elements[i].next == -1)
            num++;
    }
    return num;
}

int listDelAfter_(List *list, int index, const char * args)
{
    bool condition = index < 0 || index > list->capacity;
    // printf("%d %d %d\n", condition, index, list->capacity);

    if (condition)
    {
        PARSE_ERROR(list, condition, LIST_ERROR_INVALID_INPUT_INDEX);
        PRINT_ERR("CANNOT OPERATE: %s%s\n", __FUNCTION__, args);
        return LIST_ERROR_INVALID_INPUT_INDEX;
        
    }else if(list->elements[index].next == -1)
    {
        PARSE_ERROR(list, condition, LIST_ERROR_INVALID_INPUT_INDEX);
        PRINT_ERR("CANNOT OPERATE: %s%s\n", __FUNCTION__, args);
        return LIST_ERROR_INVALID_INPUT_INDEX;
    }

    int index_of_deleted_elem = list->elements[index].next;

    if(index_of_deleted_elem == 0)
    {
        index_of_deleted_elem = getTail(list);
        index = list->elements[index_of_deleted_elem].prev;
    }

    int index_of_elem_after_del = list->elements[index_of_deleted_elem].next;

    list->elements[index].next = index_of_elem_after_del;
    list->elements[index_of_elem_after_del].prev = index;

    fillWithPoison(list, index_of_deleted_elem);


    getNextFree(list);

    sprintf(list->operation_info, "func %s index  %s", __FUNCTION__, args);

    LIST_DUMP(list);

    return index_of_elem_after_del;

}

int listDelBefore_(List *list, int index, const char * args)
{
    char argument[64] = {};
    sprintf(argument, "prev[%s]", args);
    int tmp = listDelAfter_(list, list->elements[index].prev, argument);

    return tmp;
}

[[nodiscard]] int findElemInOrder(List *list, int logical_number)
{
    int counter1 = 1;
    for (; counter1 < list->capacity; counter1++)
    {
        if(list->elements[counter1].prev == 0)
        {
            break;
        }

    }

    // printf("find_elem_func counter1 = %d\n", counter1);

    int jumper = counter1;

    int counter2 = 1;
    for (; counter2 <= logical_number; counter2++)
    {
        int tmp = list->elements[jumper].next;
        jumper = tmp;
        // printf("jumper = %d \n", jumper);

        if(jumper == -1)
        {
            list->code_of_error |= LIST_ERROR_NO_ELEMENT_BY_THIS_LOGICAL_ORDER;
            break;
        }
    }

    // printf("counter2 = %d \n", counter2);

    return jumper;
    
}

int listSort(List *list)
{
    List_elem * sortedElements = (List_elem *)calloc(list->capacity, sizeof(List_elem));

    int jumper = getHead(list);

    int counter = 1;
    for (; jumper != 0; counter++)
    {   
        // DBG_OUT;
        sortedElements[counter].value = list->elements[jumper].value;        
        sortedElements[counter].next = counter + 1;
        sortedElements[counter].prev = counter - 1;        
        
        int tmp = list->elements[jumper].next;
        jumper = tmp;

    }

    sortedElements[0] = {.value = NIL, .next = list->elements[0].next, .prev = counter - 1};

    free(list->elements);
    list->elements = sortedElements;

    list->elements[0].prev = counter - 1;
    list->elements[counter - 1].next = 0;

    fillWithPoison(list, counter, list->capacity);

    if (getTail(list) * LIST_RESIZE_TO_SHRINK < list->capacity)
        listResize(list, list->capacity/2);

    sprintf(list->operation_info, "func %s", __FUNCTION__);

    getNextFree(list);
    LIST_DUMP(list);

    return 1;
}

int listResize(List *list, size_t new_capacity)
{
    if (new_capacity <= (list->elements[0].prev - list->elements[0].next))
        return LIST_ERROR_ATTEMPT_TO_MASH_WITH_RESIZE;

    List_elem * tmp_ptr = (List_elem *)realloc(list->elements, new_capacity * sizeof(List_elem));

    list->elements = tmp_ptr;

    fillWithPoison(list, list->capacity, new_capacity);
    list->capacity = new_capacity;

    sprintf(list->operation_info, "func %s new capacity  %ld", __FUNCTION__, new_capacity);

    getNextFree(list);

    LIST_DUMP(list);

    return 1;
}

int listDtor(List *list)
{
    free(list->elements);
   
    return 1;
}