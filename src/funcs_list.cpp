#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "list_debug.h"

#define DBG_OUT fprintf(stderr, "file: %s func: %s line: %d\n", __FILE__, __FUNCTION__, __LINE__)

int listCtor(List *list, const char * name_of_var, 
                                    const char * name_of_file, const char * name_of_func, int number_of_line)
{
//TODO FILL RANDOM SIZE OF ARRAY WITH RANDOM LETTERS
    int initial_size = 6;
    list->elements = (List_elem *)calloc(LIST_SIZE, sizeof(List_elem));

    for (int counter = 1; counter < initial_size-1; counter++)
    {
        list->elements[counter] = {.value = (elem_d)(64 + counter), .next = counter + 1, .prev = counter - 1};

    }

    list->elements[initial_size-1] = {.value = (elem_d)(64 + initial_size), .next = 0, .prev = initial_size - 2};

    for (int counter = initial_size; counter < LIST_SIZE; counter++)
    {
        list->elements[counter] = {.value =  VALUE_POISON, .next = -1, .prev = - 1};

    }

    list->code_of_error = 0;

    list->elements[0] = {.value = NIL, .next = 1, .prev = initial_size - 1};

    list->size = list->tail - list->head;
    
    list->head = 1;
    list->tail = initial_size - 1;

    stackCtor(list->free, LIST_SIZE - list->elements[0].prev);


    return 1;
}

int fillWithPoison(List *list, int beginning, int end)
{
    for (int counter = beginning; counter < end; counter++)
    {
        list->elements[counter] = {.value =  VALUE_POISON, .next = -1, .prev = - 1};
        stackPush(&list->free, counter);

    }

    return 1;
}

//TODO переписать
static int getFree(List *list, int *firstEmpty)
{
    int counter = *firstEmpty;
    for(; list->elements[counter].next != -1 && counter < LIST_SIZE; counter++)
        ;
    
    *firstEmpty = counter;

    stackPop(&list->free, firstEmpty);

    return 1;
}

static int fillWithPoison(List *list, int index)
{
    list->elements[index] = {.value = ' ', .next = -1, .prev = -1};
    
    stackPush(&list->free, index);

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

    if (index == (list->tail - 1))
    {
        list->elements[0].prev--;
        list->tail--;
        return 1;
    } 

    if( index >= list->tail)
    {
        PARSE_ERROR(list, index >= list->tail, LIST_ERROR_ATTEMPT_TO_DELETE_AFTER_TAIL);
        return LIST_ERROR_ATTEMPT_TO_DELETE_AFTER_TAIL;

    }

    return 1;   
}

static int checkForHead(List *list, int index)
{
    if (index == list->head)
    {
        list->elements[0].next++;
        list->head++;
        return 1;
    } 

    if (index < list->head)
    {
        PARSE_ERROR(list, index >= list->tail, LIST_ERROR_ATTEMPT_TO_DELETE_BEFORE_HEAD);
        return LIST_ERROR_ATTEMPT_TO_DELETE_BEFORE_HEAD;

    }

    return 1;    
}

int addElement(List *list, elem_d value, int index)
{

    list->tail++;
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

    printf("tmp_next = %d tmp_prev = %d\n", tmp_next, tmp_prev);

    swapInNext(list, index, tmp_next);
    swapInPrev(list, tmp_prev, tmp_next);

    return 1;
}

//index of element which will be deleted
int delElement(List *list, int index)
{
    checkForHead(list, index);
    checkForTail(list, index);

    swapToDel(list, index);
    fillWithPoison(list, index);

    return 1;
}

[[nodiscard]] int findElemInOrder(List *list, int logical_number)
{
    int counter1 = 1;
    for (; counter1 < LIST_SIZE; counter1++)
    {
        if(list->elements[counter1].prev == 0)
        {
            break;
        }

    }

    printf("find_elem_func counter1 = %d\n", counter1);

    int jumper = counter1;

    int counter2 = 1;
    for (; counter2 <= logical_number; counter2++)
    {
        int tmp = list->elements[jumper].next;
        jumper = tmp;
        printf("jumper = %d \n", jumper);

        if(jumper == -1)
        {
            list->code_of_error |= LIST_ERROR_NO_ELEMENT_BY_THIS_LOGICAL_ORDER;
            break;
        }
    }

    printf("counter2 = %d \n", counter2);

    return jumper;
    
}


//realloc
int listSort(List *list)
{
    List_elem * sortedElements = (List_elem *)calloc(LIST_SIZE, sizeof(List_elem));

    int jumper = list->head;

    int counter = 1;
    for (; jumper != 0; counter++)
    {   
        DBG_OUT;
        sortedElements[counter].value = list->elements[jumper].value;        
        sortedElements[counter].next = counter + 1;
        sortedElements[counter].prev = counter - 1;        
        
        int tmp = list->elements[jumper].next;
        jumper = tmp;

    }

    sortedElements[0] = {.value = NIL, .next = list->elements[0].next, .prev = counter - 1};


    free(list->elements);
    list->elements = sortedElements;

    list->tail = counter - 1;
    list->elements[counter - 1].next = 0;

    fillWithPoison(list, counter, LIST_SIZE);

    return 1;
}


int listResize(List *list)
{
    return 1;
}

int listDtor(List *list)
{
    free(list->elements);
   
    return 1;
}