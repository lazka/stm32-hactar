// Copyright 2012 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include "fat_fs.h"

#include <hactar/hactar.h>

#include <stdio.h>

void initFATFSExample(void)
{
    CardInfo info;
    hactarSDInit(&info, 512);
}
