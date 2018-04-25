#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>


#define GPIO2_BASE  0x481AC000
#define GPIO_OE	    0x134
#define GPIO_OUT    0x13C
#define PIN1	    (1<<3)

/*
 * You should execute with sudo!
 */
int main()
{
    int fd = open( "/dev/mem", O_RDWR|O_SYNC );
    if (fd < 0) {
	printf( "can't open /dev/mem \n" );
	exit( -1 );
    }


    /* 
     * void * mmap ( void *start, size_t length, int prot, int flags, int fd, off_t offset );
     * int unmap ( void *start, size_t length );
     */
    char *gpio_memory_map = (char *) mmap ( 0, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, fd, GPIO2_BASE );

    printf( "DBG: GPIO2_BASE=0x%X\n", GPIO2_BASE );		// 0x481A:C000
    printf( "DBG: gpio_memory_map=0x%X\n", gpio_memory_map );	// 0xB6EF:F000
    printf( "DBG: MAP_FAILED=0x%X\n", MAP_FAILED );		// 0xFFFF:FFFF(-1)

    if ( gpio_memory_map == MAP_FAILED )
    {
	printf( "Error : mmap \n" );
	exit (-1);
    }

    volatile unsigned int *gpio = (volatile unsigned int *) gpio_memory_map;
    gpio [ GPIO_OE / 4 ] &= (~PIN1);

    int i;
    for (i=0; i<1; i++)
    {
	gpio [ GPIO_OUT / 4 ] |= PIN1;
	sleep(1);

	gpio [ GPIO_OUT / 4 ] &= (~PIN1);
	sleep(1);
    }

    munmap ( gpio_memory_map, 4096 );

    return 0;
}
