// Copyright 2012 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#ifndef HACTER_CRC_H__
#define HACTER_CRC_H__

#include <stdint.h>
#include <stddef.h>

uint8_t crc7(uint8_t crc, uint8_t *buffer, size_t len);
uint32_t crc32(uint32_t crc, uint8_t *buffer, size_t len);

#endif
