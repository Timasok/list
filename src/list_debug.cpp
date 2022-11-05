#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list_debug.h"

static FILE * log_file;
static FILE * log;
static int pic_number = 0;
static char dump_comand[128] = {};

int verifyList(List *list)
{
    for (int idx = 0; idx < 32; idx++)
    {
        

    }
    return 1;

}

int openLogs()
{
    log_file = fopen("log_file.txt", "w+");
    setvbuf(log_file, NULL, _IONBF, 0);

    log = fopen("log.htm", "w+");
    fprintf(log, "<pre>");
    fflush(log);

    return 1;

}

int closeLogs()
{
   fclose(log_file);
   
   fprintf(log, "\n</pre>");
   fclose(log);

   return 1;
}

static void underline(List *list)
{
    fprintf(log_file, "\n          _");
    for (int counter = 0; counter < list->capacity; counter++)
    {

        fprintf(log_file, "______");
    }

}

static void printBorder(List *list)
{
    fprintf(log_file, "\n***********");
    for (int counter = 0; counter < list->capacity; counter++)
    {

        fprintf(log_file, "******");
    }

}

static void printHeadAndTail(List *list)
{
    underline(list);

    fprintf(log_file, "\n\t\t  |");
    for (size_t counter = 1; counter < list->capacity; counter++)
    {

        if (counter == list->elements[0].next + 1)
        {
            fprintf(log_file, "    H|");

        }else if (counter == list->elements[0].prev + 1 ){
            
            fprintf(log_file, "    T|");

        }else{
        
            fprintf(log_file, "     |");
        
        }

    }

}

static void printIndex(List *list)
{
    underline(list);

    fprintf(log_file, "\nINDEX  :  |");
    for (int counter = 0; counter < list->capacity; counter++)
    {
                     
        fprintf(log_file, "%5d|", counter);
 
    }
}

static void printValue(List *list)
{
    underline(list);

    fprintf(log_file, "\nVALUES :  |");
    for (int counter = 0; counter < list->capacity; counter++)
    {
        fprintf(log_file, "  %c  |", list->elements[counter].value);
        
    }
}

static void printNext(List *list)
{
    underline(list);

    fprintf(log_file, "\nNEXT   :  |");
    for (int counter = 0; counter < list->capacity; counter++)
    {
        fprintf(log_file, "%5d|", list->elements[counter].next);
        
    }

}

static void printPrev(List *list)
{
    underline(list);

    fprintf(log_file, "\nPREV   :  |");
    for (int counter = 0; counter < list->capacity; counter++)
    {
        fprintf(log_file, "%5d|", list->elements[counter].prev);
        
    }

    underline(list);
}

int graphDumpDot(List *list)
{

    //extract filename as a parameter
    FILE *graph_dump = fopen("dot_input.dot", "w");

                    // "splines = \"ortho\""
    fprintf(graph_dump, "digraph table{\n"
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
                        ", style= \"filled\", fillcolor=\"#aeb433ff\"];\n");

    for (int counter = 0; counter < list->capacity; counter++)
    {
        fprintf(graph_dump, "\t\t%d [shape = record, label = \"<i%d> %d | <v%d> VAL = %c |  <n%d> N = %d | <p%d> P = %d\" ]\n", 
                counter, counter, counter, counter, list->elements[counter].value, counter,
                     list->elements[counter].next, counter, list->elements[counter].prev);    
    }

    fprintf(graph_dump, "\t}\n");
    
    fprintf(graph_dump, "\t{\n");

    fprintf(graph_dump, "\t\tedge[style = \"invis\", weight = 1000];\n");

    fprintf(graph_dump, "\t\tH: <I>");
    for (int counter = 0; counter < list->capacity; counter++)
    {
        fprintf(graph_dump, " -> %d: <i%d>", counter, counter);   
    }

    fprintf(graph_dump, ";\n\t}\n");
    
    fprintf(graph_dump, "\t{\n");
    fprintf(graph_dump, "\t\tedge[arrowhead = normal, color = \"red\", fontsize = 12];\n\t\t");    

  
    int jumper = 0;
    int next_jumper = list->elements[0].next; 

    for (int counter = 0; counter < list->capacity && next_jumper != 0; counter++)
    {
        next_jumper = list->elements[jumper].next;

        fprintf(graph_dump, "%d: <n%d> -> %d: <n%d>", jumper, jumper, next_jumper, next_jumper);
        
        jumper = list->elements[jumper].next;

    }

    // int counter = 0;
    // do 
    // {
    //     next_jumper = list->elements[jumper].next;

    //     fprintf(graph_dump, "%d: <n%d> -> %d: <n%d>", jumper, jumper, next_jumper, next_jumper);
        
    //     jumper = list->elements[jumper].next;
    //     counter++;

    // }while (counter < list->capacity && next_jumper != list->elements[0].prev);


    fprintf(graph_dump, ";\n\t}\n}");

    fclose(graph_dump);

    sprintf(dump_comand, "dot -Tjpeg dot_input.dot > graph_dumps/dump_%d.jpeg", pic_number++);

    return 1;

}

#define PRINT_DOT(...) fprintf(graph_log, __VA_ARGS__)

int graphDumpHtml(List *list)
{
    FILE *graph_log = fopen("graph_log.html", "w");
    
    PRINT_DOT("digraph MYG {\n");
    PRINT_DOT("rankdir = LR;\n");
    PRINT_DOT("graph [splines = ortho];\n");
    PRINT_DOT("bgcolor = \"white\";\n");

    PRINT_DOT("node [shape = \"plaintext\", style = \"solid\"];\n");
    for (int i = 0; i < list->capacity; i++)
    {
        if(i == list->free_index && getSizeOfFree(list) > 0)
        {
            PRINT_DOT("free [shape = \"circle\", style = \" filled\", filcolor = \"blue\"]; \n");
            PRINT_DOT("{rank = same; free; node%d;}\n", i);
        }

        do{
        PRINT_DOT("node%d [\n"
                        "label=<\n"
                        
                        "<table border=\"0\" cellborder=\"1\" cellspacing=\"0\">\n", i);

        if (list->elements[i].next == -1)
        {
            PRINT_DOT(      "    <tr><td bgcolor=\"#dc1111\" port = \"I%d\">I = %d</td></tr>\n", i, i);

        }else{

            PRINT_DOT(      "    <tr><td bgcolor=\"yellow\" port = \"I%d\">I = %d</td></tr>\n", i, i);
        }

        PRINT_DOT(      "    <tr><td bgcolor= \"lightblue\"><font color=\"#07077a\">VALUE = %c</font></td></tr>\n"
                        
                        "    <tr>\n"
                        "    <td>\n"
                        
                        "        <table border=\"0\" cellborder=\"1\">\n"
                        
                        "        <tr>\n"
                        
                        "            <td bgcolor=\"#70de9f\">PREV = %d</td> \n"
                        
                        "            <td bgcolor = \"#c8A2c8\" port = \"N%d\"> NEXT =  %d </td>\n"
                        
                        "        </tr> \n"
                        
                        "        </table> \n"
                        
                        "    </td>\n"
                        
                        "    </tr>\n" 
                        
                        "</table>>\n"
                        "]\n\n", list->elements[i].value, list->elements[i].prev, i, list->elements[i].next);
        }while(0);

    }

    PRINT_DOT("edge [weight = \"10000\", color = \"white\"];\n");
    for (int i = 0; i < list->capacity - 1; i++)
    {
        PRINT_DOT("node%d -> node%d ", i, i+1);

    }
    PRINT_DOT(";\n");

    PRINT_DOT("edge [weight = \"1\", color = \"#3f0063\"];\n");

    int jumper = 0;
    int next_jumper = list->elements[0].next; 

    for (int counter = 0; counter < list->capacity && next_jumper != 0; counter++)
    {
        next_jumper = list->elements[jumper].next;

        // PRINT_DOT("node%d:N%d -> node%d:I%d  ", jumper, jumper, next_jumper, next_jumper);
        PRINT_DOT("node%d -> node%d ", jumper, next_jumper);

        jumper = list->elements[jumper].next;

    }
    PRINT_DOT(";\n");


    PRINT_DOT("edge [weight = \"1\", color = blue];\n");

    if(getSizeOfFree(list) > 0)
        PRINT_DOT("free -> node%ld  ", list->free_index);

    if (getSizeOfFree(list) > 1)
    {
        int counter = list->free_index;
        int idx = 0;
        while (counter < list->capacity)
        {
            if (list->elements[counter].next == -1)
            {
                idx = counter + 1;
                for (; idx < list->capacity; idx++)
                    if(list->elements[idx].next == -1)
                    {
                        PRINT_DOT("node%d-> node%d  ", counter, idx);
                        counter = idx;
                        break;
                    }
            } else 
            {
                counter++;
            }

            if (counter == list->capacity - 1)
                break;

        }
        PRINT_DOT("node%d-> node%d  ", idx, list->free_index);
        PRINT_DOT(";\n");
    }
    PRINT_DOT("\n}");

    fclose(graph_log);

    sprintf(dump_comand, "dot -Tjpeg graph_log.html > graph_dumps/dump_%d.jpeg", pic_number++);
    
    return 0;

}

#undef PRINT_DOT

int textDump(List *list, const char *name_of_file, const char *name_of_func, int number_of_line)
{
    fprintf (log_file, "\n\t\tDUMP CALLED at file: %s func: %s line: %d\n", name_of_file, name_of_func, number_of_line);

    fprintf(log_file, "{\n");
    fprintf(log_file, "size = %ld\n", list->size);
    fprintf(log_file, "elements[%p]\n" , list->elements);
    fprintf(log_file, "code_of_error = %d\n", list->code_of_error);
    fprintf(log_file, "\t}\n");

    printBorder(list);

    printHeadAndTail(list);

    printIndex(list);

    printValue(list);

    printNext(list);

    printPrev(list);

    printBorder(list);

    return 1;

}

int listDump(List *list , const char *name_of_file, const char *name_of_func, int number_of_line)
{
        
    textDump(list, name_of_file, name_of_func, number_of_line);

    // graphDumpDot(list);
    graphDumpHtml(list);

    system(dump_comand);

    fprintf(log, "<h2>Dump No. %d\n", pic_number-1);
    fprintf(log, "\t\tDUMP CALLED at file: %s func: %s line: %d\n<h2>", name_of_file, name_of_func, number_of_line);
    fprintf(log, "\t\tOPERATION INFO %s<h2>", list->operation_info);


    fprintf(log, "\n<hr>\n<img src = graph_dumps/dump_%d.jpeg>\n", pic_number-1);

    return 1;

}