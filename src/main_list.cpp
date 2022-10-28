#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "list_debug.h"

int main()
{
    openLogs();

    List list1 = {};

    LIST_CTOR(list1);

    LIST_DUMP(&list1);

// add check
    addElement(&list1, 'Y', 4);

    LIST_DUMP(&list1);

//     //check
    delElement(&list1, 3);

    LIST_DUMP(&list1);

    listSort(&list1);

    LIST_DUMP(&list1);

    listDtor(&list1);
    closeLogs();

    return 1;
}
// // add check that index < LIST_SIZE

//     LIST_DUMP(&list1);

//     int result_index = findElemInOrder(&list1, 3);
//     printf("index of element = %d\n", result_index);
