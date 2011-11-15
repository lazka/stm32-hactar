// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifndef HACTER_TERM_H__
#define HACTER_TERM_H__

typedef struct
{
    const char *command_;
    const char *description_;
    void (*function_)(void);
} TermCommand;

void startTerminal(void);

#endif
