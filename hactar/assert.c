// Copyright 2012 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include <hactar/assert.h>
#include <stdio.h>

void __hactar_assert(const char* file, int line, const char* func,
        const char* expr)
{
    fflush(stdout);
    fiprintf(stderr, "ASSERT: %s[%i]\n%s (%s)\n", file, line, func, expr);
    while(1);
}
