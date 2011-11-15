// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifndef HACTER_TERM_H__
#define HACTER_TERM_H__

#define HACTAR_TERM_INPUT_BUFFER_SIZE   50
#define HACTAR_TERM_INPUT_ARG_COUNT     10

typedef struct
{
    char *command_;
    char *description_;
    void (*function_)(char**);
} TermCommand;

void startTerminal(TermCommand* user_cmds, size_t num_user_cmds);

#endif
