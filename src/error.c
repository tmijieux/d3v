#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <glib/gi18n.h>

#include "d3v/error.h"

#define ERR_TO_STR(err)                         \
    [D3V_ERR_##err] = #err,

static char const *error_str[] = {
    D3V_ERRORS(ERR_TO_STR)
};

char const *d3v_errmsg(int error)
{
    if (error < 0)
        error = -error;
    if (error >= D3V_MAX_ERROR)
        return NULL;
    return error_str[error];
}

void d3v_warning(char const *message, ...)
{
    fprintf(stderr, _("D3V-WARNING: "));

    va_list ap;
    va_start(ap, message);
    vfprintf(stderr, message, ap);
    va_end(ap);
}

void d3v_error(char const *message, ...)
{
    fprintf(stderr, _("D3V-ERROR: "));
    
    va_list ap;
    va_start(ap, message);
    vfprintf(stderr, message, ap);
    va_end(ap);
}
