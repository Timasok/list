#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

#include "text_funcs.h"

const char* TXT_BORDER = "************************************************************";


#define ASSERT_OK(textPtr)                                              \
   do{                                                                  \
      if (returnTextError(textPtr) != 0)                                \
       {  fprintf(stderr, "DUMP_CALLED: file: %s func: %s line: %d\n",  \ 
                                 __FILE__, __FUNCTION__, __LINE__);     \
           printText(textPtr);                                          \
       }                                                                \
   } while (0)


int textCtor(Text_info * text, const char * file_name)
{

    struct stat data = {};
    stat(file_name, &data);
    text->buf_length = data.st_size;

    text->source_file = fopen(file_name, "r");     

    //ASSERT_OK(text);

    text->buf = (char *)calloc(text->buf_length + 2, sizeof(char));
    
    //ASSERT_OK(text);

    fread(text->buf, sizeof(char), text->buf_length, text->source_file);

    //ASSERT_OK(text);

    fclose(text->source_file);

    //ASSERT_OK(text);

    text->buf[text->buf_length] = '\0';

    //ASSERT_OK(text);

    text->number_of_lines = 1;
    for (size_t counter = 0;counter <= text->buf_length; counter++)
    {
        if(text->buf[counter] == '\n')
        {
            text->buf[counter] = '\0';
            text->number_of_lines++;
        }
        
    }

    //ASSERT_OK(text);

    text->lines = (const char **)calloc(text->number_of_lines, sizeof(char *));
    
    //ASSERT_OK(text);

    int buf_idx = 0;
    text->lines[0] = text->buf;
    
    //ASSERT_OK(text);

    //TODO EXTRACT DELETE COMMENTS
    for (int line = 1; line < text->number_of_lines; line++)
    {
        int tmp_buf_idx = buf_idx;
        char * tmp_comment = strchr(&text->buf[tmp_buf_idx], ';');

        while (text->buf[buf_idx++] != '\0')
            ;

        if (tmp_comment != nullptr && tmp_comment < &text->buf[buf_idx])
            *tmp_comment = '\0';

        text->lines[line] = text->buf + buf_idx;
        
    }

    //ASSERT_OK(text);

    return 0;
}

int printText(Text_info * text)
{
    fprintf(stderr, "\n%s\n", TXT_BORDER);
    fprintf(stderr, "  code_of_error = %d\n", text->code_of_error);
    fprintf(stderr, "  buf_length = %ld\n  number_of_lines = %d\n  buf: \n%s\n", text->buf_length, text->number_of_lines, text->buf);

    if (text->number_of_lines != 0)
        fprintf(stderr, "\tLines: \n");
    for (int counter = 0; counter < text->number_of_lines; counter++)
        printf("%s\n", text->lines[counter]);

    fprintf(stderr, "\n%s\n", TXT_BORDER);

    return 0;
}


int returnTextError(Text_info * text)
{

    text->code_of_error |= ((!text->source_file) * TEXT_ERROR_SOURCE_FILE_IS_NULL);

    text->code_of_error |= ((!text->lines) * TEXT_ERROR_LINES_IS_NULL);

    text->code_of_error |= ((text->number_of_lines < 1) * TEXT_ERROR_NUMBER_OF_LINES_IS_INAPROPRIATE);

    text->code_of_error |= ((!text->buf) * TEXT_ERROR_BUF_IS_NULL);

    return text->code_of_error;

}

#undef ASSERT_OK