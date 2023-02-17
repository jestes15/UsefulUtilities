// SYSTEM INCLUDE FILES
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/types.h>

// C STANDARD LIBRARY INCLUDE FILES
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

enum file_operations
{
    FAILED_TO_OPEN_FILE = 1,
    FAILED_TO_READ_FILE,
    FAILED_TO_WRITE_FILE,
};

enum return_codes
{
    SUCCESS = 0,
    FAILURE = 1,
};

/*
* operating modes
* 0x01 - copy using user space
* 0x02 - copy using sendfile
* 0x04 - copy using mmap
*/

u_int8_t mode = 0;

#define ENABLE_USER_SPACE_COPY(x) x &= 0x01
#define ENABLE_KERNEL_SPACE_COPY(x) x &= 0x02
#define ENABLE_MMAP_COPY(x) x &= 0x04

#define CHECK_USER_SPACE_COPY_BIT(x) x & 0x01
#define CHECK_KERNEL_SPACE_COPY_BIT(x) x & 0x02
#define CHECK_MMAP_COPY_BIT(x) x & 0x04