#include <stdio.h>
#include <stdlib.h>
#include "linear.h"

static char *line = NULL;
static int max_line_len;

static char* readline(FILE *input);

void print_null(const char *s);

void exit_input_error(int line_num);
