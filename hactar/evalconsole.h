// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifndef HACTAR_EVALCONSOLE_H__
#define HACTAR_EVALCONSOLE_H__

#include "stddef.h"
#include "stdout.h"

typedef struct
{
    StdoutDevice device_;
    size_t line_;
    size_t column_;
} EvalLCDConsoleInfo;

EvalLCDConsoleInfo evallcdconsole_info;

void initEvalLCDStdoutDevice(void);

#endif
