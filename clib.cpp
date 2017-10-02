#include <sys/stat.h>

#include "uart.h"
#include "system.h"

//Needed for Clib

extern "C"
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

extern "C"
int _write( int file, char *ptr, int len )
{
    System::uart().write(ptr, len);
    
    return len;
}

extern "C"
int _close( int file )
{
    return -1;
}

extern "C"
int _fstat( int file, struct stat *st )
{
    st->st_mode = S_IFCHR;
    return 0;
}

extern "C"
int _isatty(int file)
{
    return 1;
}

extern "C"
int _lseek(int file, int ptr, int dir)
{
    return 0;
}

extern "C"
int _read( int file, char *ptr, int len )
{
    return 0;
}

//Needed for libstdc++

#include <errno.h>
#undef errno
extern int errno;

void _exit( int status )
{
    /* Stop the compiler complaining about unused variables by "using" it */
    (void)status;
    
    while(1)
    {
        /* TRAP HERE */
    }
}

extern "C"
int _kill( int pid, int sig )
{
    errno = EINVAL;
    return -1;
}

extern "C"
int _getpid( void )
{
    return 1;
}

//For global objects
extern "C"
int _open( const char *name, int flags, int mode )
{
    return -1;
}