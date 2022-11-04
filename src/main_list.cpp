#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "list_debug.h"

int main()
{
    openLogs();

    List list1 = {};  

    LIST_CTOR(list1, 7);
    
    listAddAfter(&list1, 'T', 2);
    listAddAfter(&list1, 'E', 2);
    listAddAfter(&list1, 'M', 2);
    listAddAfter(&list1, 'A', 2);
    listAddAfter(&list1, 'S', 2);

    listDelAfter(&list1, 4);

    listSort(&list1);

    listDtor(&list1);

    closeLogs();

    return 1;
}