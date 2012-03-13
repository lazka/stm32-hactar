// Copyright 2012 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifndef HACTER_ASSERT_H__
#define HACTER_ASSERT_H__

#ifdef NDEBUG
#define assert(expr) ((void)0)
#else
#define assert(expr) ((expr) ? (void)0 : __hactar_assert(__FILE__, __LINE__, __func__, #expr))
#endif

void __hactar_assert(const char* file, int line, const char* func, const char* expr);

#endif
