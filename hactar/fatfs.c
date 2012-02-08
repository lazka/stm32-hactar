// Copyright 2012 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include <hactar/platform_check.h>

#ifdef HACTAR_USE_FATFS

#include <hactar/hactar.h>
#include "ffconf.h"
#include <diskio.h>

static DSTATUS status = STA_NOINIT | STA_NODISK;
static CardInfo info;

DSTATUS disk_initialize(BYTE drive)
{
    // Already initialized
    if(!(status & STA_NOINIT) && !(status & STA_NODISK))
        return status;

    status = STA_NOINIT | STA_NODISK;

    if(hactarSDInit(&info, _MAX_SS) != 0)
        return status;

    // Delete the flags
    status &= ~STA_NOINIT;
    status &= ~STA_NODISK;

    return status;
}

DSTATUS disk_status(BYTE drive)
{
    if(drive)
        return STA_NOINIT | STA_NODISK;

    return status;
}

DRESULT disk_read(BYTE drive, BYTE* buffer,
        DWORD sector_number, BYTE sector_count)
{
    if(drive)
        return RES_PARERR;

    if(status & STA_NOINIT)
        return RES_NOTRDY;

    if(hactarSDReadBlocks(&info, sector_number, sector_count, buffer) != 0)
        return RES_ERROR;

    return RES_OK;
}

DRESULT disk_write(BYTE drive, const BYTE* buffer,
        DWORD sector_number, BYTE sector_count)
{
    if(drive)
        return RES_PARERR;

    if(status & STA_NOINIT)
        return RES_NOTRDY;

    if(hactarSDWriteBlocks(&info, sector_number, sector_count, buffer) != 0)
        return RES_ERROR;

    return RES_OK;
}

DRESULT disk_ioctl(BYTE drive, BYTE command, void* buffer)
{
    if(drive || (status & STA_NOINIT))
        return RES_NOTRDY;

    if(command == CTRL_SYNC)
    {
        return RES_ERROR;
    }
    else if(command == GET_SECTOR_SIZE)
    {
        *(WORD*)buffer = info.block_size_;
        return RES_OK;
    }
    else if(command == GET_SECTOR_COUNT)
    {
        *(DWORD*)buffer = info.block_count_;
        return RES_OK;
    }
    else if(command == GET_BLOCK_SIZE)
    {
        *(DWORD*)buffer = info.erase_sectors_count_;
        return RES_OK;
    }
    else
        return RES_PARERR;
}

DWORD get_fattime(void)
{
    return 0;
}

#endif
