// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include "misc.h"

void interruptsDisable()
{
    asm volatile ("cpsid   i" : : : "memory");
}

void interruptsEnable()
{
    asm volatile ("cpsie   i" : : : "memory");
}
