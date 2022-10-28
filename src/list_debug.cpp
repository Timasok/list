#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list_debug.h"

static FILE * log_file;
static int pic_number = 0;
static char dump_comand[100] = "dot -Tjpeg dot_input.txt  > dump_0.jpeg";

static char * strTripleCat(char * s1, const char *s2, const char *s3)
{
    strcat(s1, s2);
    strcat(s1, s3);

    return s1;
}

int openLogs()
{
   log_file = fopen("log_file.txt", "w+");
   setvbuf(log_file, NULL, _IONBF, 0);
   return 1;
}

int closeLogs()
{
   fclose(log_file);
   return 1;
}

static void underline()
{
    fprintf(log_file, "\n          _");
    for (int counter = 0; counter < LIST_SIZE; counter++)
    {

        fprintf(log_file, "______");
    }

}

static void printBorder()
{
    fprintf(log_file, "\n***********");
    for (int counter = 0; counter < LIST_SIZE; counter++)
    {

        fprintf(log_file, "******");
    }

}

static void printHeadAndTail(List *list)
{
    underline();

    fprintf(log_file, "\n\t\t  |");
    for (size_t counter = 0; counter < LIST_SIZE; counter++)
    {

        if (counter == list->head)
        {
            fprintf(log_file, "    H|");

        }else if (counter == list->tail){
            
            fprintf(log_file, "    T|");

        }else{
        
            fprintf(log_file, "     |");
        
        }

    }

}

static void printIndex(List *list)
{
    underline();

    fprintf(log_file, "\nINDEX  :  |");
    for (int counter = 0; counter < LIST_SIZE; counter++)
    {
                     
        fprintf(log_file, "%5d|", counter);
 
    }
}

static void printValue(List *list)
{
    underline();

    fprintf(log_file, "\nVALUES :  |");
    for (int counter = 0; counter < LIST_SIZE; counter++)
    {
        fprintf(log_file, "  %c  |", list->elements[counter].value);
        
    }
}

static void printNext(List *list)
{
    underline();

    fprintf(log_file, "\nNEXT   :  |");
    for (int counter = 0; counter < LIST_SIZE; counter++)
    {
        fprintf(log_file, "%5d|", list->elements[counter].next);
        
    }

}

static void printPrev(List *list)
{
    underline();

    fprintf(log_file, "\nPREV   :  |");
    for (int counter = 0; counter < LIST_SIZE; counter++)
    {
        fprintf(log_file, "%5d|", list->elements[counter].prev);
        
    }

    underline();
}

static void printHeadForPic(FILE *graph_dump)
{
    fprintf(graph_dump, "digraph table{\n"
                        "splines = \"ortho\""
                        "\t size = \"300, 300\";\n"
                        "\trankdir = LR;\n");

    fprintf(graph_dump,
                        "\t{\n"
                        "\t\tnode[shape = \"rectangle\", style = \"rounded\", color = \"lightblue\"];\n"
                        "\t\tH [shape = record, label = \"<I> INDEX |<V>  VALUE |<N>  NEXT |<P> PREV \"];\n"
                        "\t}\n");

    fprintf(graph_dump,
                        "\t{\n"
                        "\t\tnode[shape = \"rectangle\", style = \"rounded\""
                        ", style= \"filled\", fillcolor=\"lightgreen\"];\n");

}

int listDump(List *list, const char *name_of_file, const char *name_of_func, int number_of_line)
{
    fprintf (log_file, "\n\t\tDUMP CALLED at file: %s func: %s line: %d\n", name_of_file, name_of_func, number_of_line);

    fprintf(log_file, "{\n");
    fprintf(log_file, "size = %ld\n", list->size);
    fprintf(log_file, "elements[%p]\n" , list->elements);
    fprintf(log_file, "code_of_error = %d\n", list->code_of_error);
    fprintf(log_file, "\t}\n");

    printBorder();

    printHeadAndTail(list);

    printIndex(list);

    printValue(list);

    printNext(list);

    printPrev(list);

    printBorder();
    
    //extract filename as a parameter
    FILE *graph_dump = fopen("dot_input.txt", "w");

    printHeadForPic(graph_dump);
    
    for (int counter = 0; counter < LIST_SIZE; counter++)
    {
        fprintf(graph_dump, "\t\t%d [shape = record, label = \"<i%d> %d | <v%d> VAL = %c | { <n%d> N = %d | <p%d> P = %d}\" ]\n", 
                counter, counter, counter, counter, list->elements[counter].value, counter,
                     list->elements[counter].next, counter, list->elements[counter].prev);    
    }

    fprintf(graph_dump, "\t}\n");
    
    fprintf(graph_dump, "\t{\n");

    fprintf(graph_dump, "\t\tedge[style = \"invis\", weight = 100000];\n");

    fprintf(graph_dump, "\t\tH: <I>");
    for (int counter = 0; counter < LIST_SIZE; counter++)
    {
        fprintf(graph_dump, " -> %d: <i%d>", counter, counter);   
    }    

    fprintf(graph_dump, ";\n\t}\n");
    
    fprintf(graph_dump, "\t{\n");
    fprintf(graph_dump, "\t\tedge[arrowhead = normal, color = \"red\", fontsize = 12];\n");    

    int index = findElemInOrder(list, 0);
    fprintf(graph_dump, "\t\t%d: <n%d>", index, index);
    
    for (int counter = 1; counter < LIST_SIZE; counter++)
    {
        index = findElemInOrder(list, counter);

        if(index == 0)
            break;

        fprintf(graph_dump, " -> %d: <n%d>", index, index);   
        
    }

    fprintf(graph_dump, ";\n\t}\n}");

    fclose(graph_dump);
    
    pic_number++;
    sprintf(dump_comand, "dot -Tjpeg dot_input.txt  > dump_%d.jpeg", pic_number);
    printf("pic_number = %d, %c\n", pic_number, dump_comand[33]);

    system(dump_comand);

    return 1;

}