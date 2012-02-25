// Copyright 2011 Christoph Reiter
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//

#include <reent.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <hactar/misc.h>
#include <hactar/stdio_dev.h>

int _kill(int pid, int sig)
{
    errno = EINVAL;
    return -1;
}

void _exit(int status)
{
    while(1) {;}
}

int _getpid(void)
{
    return 1;
}

caddr_t _sbrk(int incr)
{
    extern char _end;
    static char *heap_end;

    char *prev_heap_end;
    if (heap_end == 0) {
        heap_end = &_end;
    }
    prev_heap_end = heap_end;

    heap_end += incr;
    return (caddr_t) prev_heap_end;
}

int _close(int file)
{
    return -1;
}

int _fstat(int file, struct stat *st)
{
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int file)
{
    return 1;
}

int _lseek(int file, int ptr, int dir) {
    return 0;
}

int _read(int file, char *ptr, int len)
{
    if(file == STDIN_FILENO)
    {
        if(stdin_device != NULL)
            return stdin_device->read_func_(ptr, len);
    }
    else
        assert(0);

    return 0;
}

int _write(int file, char *ptr, int len)
{
    if(file == STDOUT_FILENO || file == STDERR_FILENO)
    {
        if(stdout_device != NULL)
            return stdout_device->write_func_(ptr, len, file == STDERR_FILENO);
    }
    else
        assert(0);

    return len;
}
