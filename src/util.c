#include <stdio.h>
#include "d3v/util.h"
#include "d3v/error.h"

unsigned d3v_strsplit_size(char **split)
{
    unsigned count = 0;
    while (split[count] != NULL)
        ++ count;
    return count;
}

int d3v_readline(GIOChannel *chan, char **return_value)
{
    int err = 0;
    char *str = NULL;
    size_t length, end;
    GError *error = NULL;
    
    *return_value = NULL;
    
    GIOStatus status = g_io_channel_read_line(chan, &str, &length, &end, &error);
    if (status == G_IO_STATUS_EOF)
        return 1;
    else if (status != G_IO_STATUS_NORMAL) {
        printf ("getline: %s\n", error?error->message:"no error");
        g_free(str);
        g_clear_error(&error);
        return -D3V_ERR_FILE_BAD_ENCODING;
    }
    str[end] = 0;
    g_strstrip(str);
    *return_value = str;
    return err;
}
