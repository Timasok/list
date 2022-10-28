#ifndef LIST_DEBUG_FUNCS_H
#define LIST_DEBUG_FUNCS_H

#include "list.h"

#define LIST_DUMP(listPtr)                                                                      \
        listDump(listPtr, __FILE__, __FUNCTION__,__LINE__)

#define PARSE_ERROR(listPtr, condition, error_specifier)                                        \
        list->code_of_error  |= ((condition) & error_specifier);


int listDump(List *list, const char *name_of_file, const char *name_of_func, int number_of_line);

int openLogs();
int closeLogs();

enum LIST_ERRORS
{
        LIST_ERROR_BUFFER_OVERFLOW                      = 1 << 0,
        LIST_ERROR_WRONG_TAIL_USAGE                     = 1 << 1,
        LIST_ERROR_INVALID_INPUT_INDEX                  = 1 << 2,
        LIST_ERROR_ATTEMPT_TO_DELETE_AFTER_TAIL         = 1 << 3,
        LIST_ERROR_ATTEMPT_TO_DELETE_BEFORE_HEAD        = 1 << 4,
        LIST_ERROR_NO_ELEMENT_BY_THIS_LOGICAL_ORDER     = 1 << 5,
};

#endif 