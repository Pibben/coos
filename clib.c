#include <sys/stat.h>

#include "uart.h"

caddr_t _sbrk( int incr )
{
    extern char __end;
    static char* heap_end = 0;
    char* prev_heap_end;
    
    if( heap_end == 0 )
        heap_end = &__end;
    
    prev_heap_end = heap_end;
    
    heap_end += incr;
    return (caddr_t)prev_heap_end;
}

int _write( int file, char *ptr, int len )
{
    uart_write(ptr, len);
    
    return len;
}

int _close( int file )
{
    return -1;
}

int _fstat( int file, struct stat *st )
{
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int file)
{
    return 1;
}

int _lseek(int file, int ptr, int dir)
{
    return 0;
}

int _read( int file, char *ptr, int len )
{
    return 0;
}
