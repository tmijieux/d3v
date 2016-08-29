#ifndef D3V_UTIL_H
#define D3V_UTIL_H

#include <stdlib.h>
#include <glib.h>

#define ARRAY_BUF_SIZE(array_var, array_size)      \
    ((sizeof(*(array_var)) * (array_size)))

#define ALLOC_ARRAY(array_var, array_size)      \
    ((array_var) = g_malloc(ARRAY_BUF_SIZE(array_var, array_size)))

#define ALLOC_ARRAY0(array_var, array_size)      \
    ((array_var) = g_malloc0(ARRAY_BUF_SIZE(array_var, array_size)))

unsigned d3v_strsplit_size(char **split);
int d3v_readline(GIOChannel *chan, char **return_value);

#endif // D3V_UTIL_H
