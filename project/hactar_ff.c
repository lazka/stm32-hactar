// Copyright 2012 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include <hactar/hactar.h>
#include <diskio.h>

static volatile DSTATUS status = STA_NOINIT; // STA_NOINIT, STA_NODISK, STA_PROTECTED

DSTATUS disk_initialize(BYTE drive)
{
    return status;
}

DSTATUS disk_status(BYTE drive)
{
    return status;
}

DRESULT disk_read(BYTE drive, BYTE* buffer,
        DWORD sector_number, BYTE sector_count)
{
    return RES_ERROR;
}

DRESULT disk_write(BYTE drive, const BYTE* buffer,
        DWORD sector_number, BYTE sector_count)
{
    return RES_ERROR;
}

DRESULT disk_ioctl(BYTE drive, BYTE command, void* buffer)
{
    if(command == CTRL_SYNC)
    {

    }
    else if(command == GET_SECTOR_SIZE)
    {

    }
    else if(command == GET_SECTOR_COUNT)
    {

    }
    else if(command == GET_BLOCK_SIZE)
    {

    }
    else
        return RES_PARERR;

    return RES_ERROR; // RES_NOTRDY, RES_ERROR, RES_OK
}

DWORD get_fattime(void)
{
    return 0;
}
