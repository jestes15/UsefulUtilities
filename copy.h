// SYSTEM INCLUDE FILES
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/sendfile.h>
#include <sys/mman.h>

// C STANDARD LIBRARY INCLUDE FILES
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum file_operations
{
    FAILED_TO_OPEN_FILE = 11,
    FAILED_TO_READ_FROM_FILE,
    FAILED_TO_WRITE_TO_FILE,
    FAILED_TO_CLOSE_FILE
};

enum mmap_operations
{
    FAILED_TO_MMAP_SOURCE_FILE = 15,
    FAILED_TO_MMAP_DESTINATION_FILE,
    FAILED_TO_MUNMAP_SOURCE_FILE,
    FAILED_TO_MUNMAP_DESTINATION_FILE,
    FAILED_TO_SYNC_DESTINATION_FILE
};

enum return_codes
{
    SUCCESS = 0,
    ILLEGAL_OPTIONS_PASSED,
    SOURCE_FILE_NOT_VALID,
    NO_SOURCE_PATH_PROVIDED,
    DESTINATION_FILE_NOT_VALID,
    NO_DESTINATION_PATH_PROVIDED,
    DESTINATION_FILE_ALREADY_EXISTS,
    NO_SOURCE_OR_DESTINATION_PATH_PROVIDED,
    USER_SPACE_COPY_FAILED,
    KERNEL_SPACE_COPY_FAILED,
    MMAP_COPY_FAILED
};

/*
 * operating modes
 * 0x01 - copy using user space
 * 0x02 - copy using sendfile
 * 0x04 - copy using mmap
 * 0x08 - enable force write
 * 0x10 - enable file protection
 */

u_int8_t mode = 0;

#define ENABLE_USER_SPACE_COPY(x) (x |= 0x01)
#define ENABLE_KERNEL_SPACE_COPY(x) (x |= 0x02)
#define ENABLE_MMAP_COPY(x) (x |= 0x04)
#define ENABLE_FORCE_WRITE(x) (x |= 0x08)
#define ENABLE_FILE_PROTECTION(x) (x |= 0x10)

#define CHECK_USER_SPACE_COPY_BIT(x) ((x & 0x01) == 0x01)
#define CHECK_KERNEL_SPACE_COPY_BIT(x) ((x & 0x02) == 0x02)
#define CHECK_MMAP_COPY_BIT(x) ((x & 0x04) == 0x04)
#define CHECK_FORCE_WRITE(x) ((x & 0x08) == 0x08)
#define CHECK_FILE_PROTECTION(x) ((x & 0x10) == 0x10)

/*
 * Path Bits
 * 0x01 - Source path set
 * 0x02 - Source path set resulted in an error
 * 0x04 - Destination path set
 * 0x08 - Destination path set resulted in an error
 */

u_int8_t path_set;

#define SOURCE_BIT_SET(x) (x |= 0x01)
#define SOURCE_ERROR_BIT_SET(x) (x |= 0x02)
#define DEST_BIT_SET(x) (x |= 0x04)
#define DEST_ERROR_BIT_SET(x) (x |= 0x08)

#define CHECK_SOURCE_BIT(x) ((x & 0x01) == 0x01)
#define CHECK_SOURCE_ERROR_BIT(x) ((x & 0x02) == 0x02)
#define CHECK_DEST_BIT(x) ((x & 0x04) == 0x04)
#define CHECK_DEST_ERRPR_BIT(x) ((x & 0x08) == 0x08)

int user_space_copy(char *, char *);
int kernel_space_copy(char *, char *);
int mmap_copy(char *, char *);