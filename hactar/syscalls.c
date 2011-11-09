/*
 * syscalls.c
 *
 *  Created on: 03.12.2009
 *      Author: Martin Thomas, 3BSD license
 */

#include <reent.h>
#include <errno.h>
#include <stdlib.h> /* abort */
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "stm32f10x.h" /* for _get_PSP() from core_cm3.h*/
#include "stm32_eval.h"

#include "misc.h"

#undef errno
extern int errno;

int _kill(int pid, int sig)
{
    pid = pid; sig = sig; /* avoid warnings */
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


extern char _end; /* Defined by the linker */
static char *heap_end;

char* get_heap_end(void)
{
    return (char*) heap_end;
}

char* get_stack_top(void)
{
    return (char*) __get_MSP();
    // return (char*) __get_PSP();
}

caddr_t _sbrk(int incr)
{
    char *prev_heap_end;
    if (heap_end == 0) {
        heap_end = &_end;
    }
    prev_heap_end = heap_end;
#if 1
    if (heap_end + incr > get_stack_top()) {
        abort();
    }
#endif
    heap_end += incr;
    return (caddr_t) prev_heap_end;
}

int _close(int file)
{
    file = file; /* avoid warning */
    return -1;
}

int _fstat(int file, struct stat *st)
{
    file = file; /* avoid warning */
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int file)
{
    file = file; /* avoid warning */
    return 1;
}

int _lseek(int file, int ptr, int dir) {
    file = file; /* avoid warning */
    ptr = ptr; /* avoid warning */
    dir = dir; /* avoid warning */
    return 0;
}

int _read(int file, char *ptr, int len)
{
    file = file; /* avoid warning */
    ptr = ptr; /* avoid warning */
    len = len; /* avoid warning */
    return 0;
}

int _write(int file, char *ptr, int len)
{
    size_t n;
    if(file == STDOUT_FILENO)
    {
        for (n = 0; n < len; n++)
        {
            USART_SendData(EVAL_COM1, (uint8_t) *ptr++);
            while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET);
        }
    }
    else
        assert(0);

    return len;
}
