#ifndef LIST_DEBUG_FUNCS_H
#define LIST_DEBUG_FUNCS_H

#include "list.h"

#define LIST_DUMP(listPtr)                                                                \
        listDump(listPtr, __FILE__, __FUNCTION__,__LINE__)

#define PARSE_ERROR(listPtr, condition, error_specifier)                                        \
        listPtr->code_of_error  |= ((condition) & error_specifier);

#define PRINT_ERROR(error_specifier)                                        \
        fprintf(log, "%s", #error_specifier);

int verifyList(List *list);

int openLogs();
int closeLogs();

int textDump      (List *list, const char *name_of_file, const char *name_of_func, int number_of_line);
int listDump      (List *list, const char *name_of_file, const char *name_of_func, int number_of_line);
int graphDumpDot(List *list);
int graphDumpHtml(List *list);

#define DEF_ERR(name, num) \
            LIST_ERROR_##name = 1 << num,

enum LIST_ERRORS
{
        #include "comands.h"
};

#undef DEF_ERR
#endif 