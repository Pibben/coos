extern int __bss_start;
extern int __bss_end;

extern void kernel_main( unsigned int r0, unsigned int r1, unsigned int atags );
extern void __libc_init_array();

void _init() {}

void _cstartup( unsigned int r0, unsigned int r1, unsigned int r2 )
{
    int* bss = &__bss_start;
    int* bss_end = &__bss_end;

    /* Clear the BSS section */
    while( bss < bss_end )
        *bss++ = 0;

    __libc_init_array(); //Call static constructors

    /* We should never return from main ... */
    kernel_main( r0, r1, r2 );

    /* ... but if we do, safely trap here */
    while(1)
    {
        /* EMPTY! */
    }
}
