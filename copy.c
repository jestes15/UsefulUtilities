#include "copy.h"

// TODO: Add support for copying multiple files to a directory
int main(int argc, char *argv[])
{
    u_int8_t argument, index = 1;
    char *source = NULL, *dest = NULL;

    for (argument = 1; argument < argc; ++argument)
    {
        if (argv[argument][0] == '-')
        {
            u_int8_t len_of_arg = strlen(argv[argument]);
            for (index = 1; index < len_of_arg; ++index)
            {
                switch (argv[argument][index])
                {
                case 'h':
                    printf("Usage: copy [OPTION] SOURCE DEST\n");
                    printf("Copy SOURCE to DEST, or multiple SOURCE(s) to DIRECTORY.\n\n");
                    printf("  -h\tDisplay this help and exit\n");
                    printf("  -u\tUse the user space copy method\n");
                    printf("  -k\tUse the kernel space copy method\n");
                    printf("  -m\tUse the mmap copt method\n");
                    printf("  -f\tForce overwrite of DEST if it already exists\n");
                    printf("  -n\tDo not overwrite an existing file\n\n");
                    printf("If no options are specified, the default is to use the user space copy method and not overwrite files\n");
                    exit(SUCCESS);
                    break;
                case 'u':
                    if (CHECK_KERNEL_SPACE_COPY_BIT(mode) || CHECK_MMAP_COPY_BIT(mode))
                    {
                        printf("ERROR: Illegal argument passed: %c\n", argv[argument][index]);
                        printf("Can not use k or m options with option u\n");
                        exit(ILLEGAL_OPTIONS_PASSED);
                    }
                    ENABLE_USER_SPACE_COPY(mode);
                    break;
                case 'k':
                    if (CHECK_USER_SPACE_COPY_BIT(mode) || CHECK_MMAP_COPY_BIT(mode))
                    {
                        printf("ERROR: Illegal argument passed: %c\n", argv[argument][index]);
                        printf("Can not use u or m options with option k\n");
                        exit(ILLEGAL_OPTIONS_PASSED);
                    }
                    ENABLE_KERNEL_SPACE_COPY(mode);
                    break;
                case 'm':
                    if (CHECK_USER_SPACE_COPY_BIT(mode) || CHECK_KERNEL_SPACE_COPY_BIT(mode))
                    {
                        printf("ERROR: Illegal argument passed: %c\n", argv[argument][index]);
                        printf("Can not use u or k options with option m\n");
                        exit(ILLEGAL_OPTIONS_PASSED);
                    }
                    ENABLE_MMAP_COPY(mode);
                    break;
                case 'f':
                    if (CHECK_FILE_PROTECTION(mode))
                    {
                        printf("ERROR: Illegal argument passed: %c\n", argv[argument][index]);
                        printf("Can not use option n with option m\n");
                        exit(ILLEGAL_OPTIONS_PASSED);
                    }
                    ENABLE_FORCE_WRITE(mode);
                    break;
                case 'n':
                    if (CHECK_FORCE_WRITE(mode))
                    {
                        printf("ERROR: Illegal argument passed: %c\n", argv[argument][index]);
                        printf("Can not use option n with option m\n");
                        exit(ILLEGAL_OPTIONS_PASSED);
                    }
                    ENABLE_FILE_PROTECTION(mode);
                    break;
                default:
                    printf("ERROR: Illegal argument passed: %c\n", argv[argument][index]);
                    printf("Correct usage is: copy [OPTION] SOURCE DEST\n");
                    break;
                }
            }
        }

        else if (!CHECK_SOURCE_BIT(path_set))
        {
            source = argv[argument];
            SOURCE_BIT_SET(path_set);
        }

        else if (!CHECK_DEST_BIT(path_set))
        {
            dest = argv[argument];
            DEST_BIT_SET(path_set);
        }
    }

    if (!CHECK_SOURCE_BIT(path_set) || !CHECK_DEST_BIT(path_set))
    {
        printf("ERROR: Source or destination path not provided\n");
        exit(NO_SOURCE_OR_DESTINATION_PATH_PROVIDED);
    }

    if (faccessat(AT_FDCWD, source, F_OK | R_OK | W_OK, AT_SYMLINK_NOFOLLOW) != 0)
    {
        printf("ERROR: Source file does not exist or is not readable\n");
        exit(SOURCE_FILE_NOT_VALID);
    }

    if (faccessat(AT_FDCWD, dest, F_OK | R_OK | W_OK, AT_SYMLINK_NOFOLLOW) == 0)
    {
        if (CHECK_FORCE_WRITE(mode))
        {
            printf("WARNING: Destination file already exists and will be overwritten\n");
        }
        else if (CHECK_FILE_PROTECTION(mode))
        {
            printf("ERROR: Destination file already exists\n");
            exit(DESTINATION_FILE_ALREADY_EXISTS);
        }
    }

    if (CHECK_USER_SPACE_COPY_BIT(mode))
    {
        if (user_space_copy(source, dest) != SUCCESS)
        {
            printf("ERROR: User space copy failed\n");
            exit(USER_SPACE_COPY_FAILED);
        }
    }
    else if (CHECK_KERNEL_SPACE_COPY_BIT(mode))
    {
        if (kernel_space_copy(source, dest) != SUCCESS)
        {
            printf("ERROR: Kernel space copy failed\n");
            exit(KERNEL_SPACE_COPY_FAILED);
        }
    }
    else if (CHECK_MMAP_COPY_BIT(mode))
    {
        if (mmap_copy(source, dest) != SUCCESS)
        {
            printf("ERROR: Mmap copy failed\n");
            exit(MMAP_COPY_FAILED);
        }
    }
    else
    {
        if (user_space_copy(source, dest) != SUCCESS)
        {
            printf("ERROR: User space copy failed\n");
            exit(USER_SPACE_COPY_FAILED);
        }
    }
}

int user_space_copy(char *src, char *dest)
{
    int source_fd, dest_fd;
    char buffer[1024];
    ssize_t bytes_read, bytes_written;

    if ((source_fd = open(src, O_RDONLY)) < 0)
    {
        printf("ERROR: Failed to open source file: %s\n", strerror(errno));
        return FAILED_TO_OPEN_FILE;
    }

    if ((dest_fd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0)
    {
        printf("ERROR: Failed to open destination file: %s\n", strerror(errno));
        return FAILED_TO_OPEN_FILE;
    }

    while ((bytes_read = read(source_fd, buffer, sizeof(buffer))) > 0)
    {
        if ((bytes_written = write(dest_fd, buffer, bytes_read)) != bytes_read)
        {
            printf("ERROR: Failed to write to destination file: %s\n", strerror(errno));
            return FAILED_TO_WRITE_TO_FILE;
        }
    }

    if (bytes_read < 0)
    {
        printf("ERROR: Failed to read from source file: %s\n", strerror(errno));
        return FAILED_TO_READ_FROM_FILE;
    }

    if (close(source_fd) < 0)
    {
        printf("ERROR: Failed to close source file: %s\n", strerror(errno));
        return FAILED_TO_CLOSE_FILE;
    }

    if (close(dest_fd) < 0)
    {
        printf("ERROR: Failed to close destination file: %s\n", strerror(errno));
        return FAILED_TO_CLOSE_FILE;
    }

    return SUCCESS;
}

int kernel_space_copy(char *src, char *dest)
{
    int source_fd, dest_fd, error;
    struct stat st;
    off_t offset = 0, size_of_file = 0;
    int rv = 0;

    stat(src, &st);
    size_of_file = st.st_size;

    if ((source_fd = open(src, O_RDONLY)) < 0)
    {
        printf("ERROR: Failed to open source file: %s\n", strerror(errno));
        return FAILED_TO_OPEN_FILE;
    }

    if ((dest_fd = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0)
    {
        printf("ERROR: Failed to open destination file: %s\n", strerror(errno));
        return FAILED_TO_OPEN_FILE;
    }

    if ((rv = sendfile(dest_fd, source_fd, &offset, size_of_file)) < 0)
    {
        if ((error = errno) != 0)
        {
            printf("Warning: sendfile(3EXT) returned %d (errno %d)\n", rv, errno);
            printf("%s\n", strerror(errno));
        }
    }

    if (close(source_fd) < 0)
    {
        printf("ERROR: Failed to close source file: %s\n", strerror(errno));
        return FAILED_TO_CLOSE_FILE;
    }

    if (close(dest_fd) < 0)
    {
        printf("ERROR: Failed to close destination file: %s\n", strerror(errno));
        return FAILED_TO_CLOSE_FILE;
    }

    return SUCCESS;
}

int mmap_copy(char *src, char *dest)
{
    int source_fd, dest_fd;

    if ((source_fd = open(src, O_RDONLY)) < 0)
    {
        printf("ERROR: Failed to open source file: %s\n", strerror(errno));
        return FAILED_TO_OPEN_FILE;
    }

    if ((dest_fd = open(dest, O_RDWR | O_CREAT | O_TRUNC, 0644)) < 0)
    {
        printf("ERROR: Failed to open destination file: %s\n", strerror(errno));
        return FAILED_TO_OPEN_FILE;
    }

    struct stat src_sz;
    stat(src, &src_sz);
    off_t size_of_file_source = src_sz.st_size;

    lseek(dest_fd, size_of_file_source - 1, SEEK_SET);
    write(dest_fd, "", 1);
    lseek(dest_fd, 0, SEEK_SET);

    char *src_map = mmap(NULL, size_of_file_source, PROT_READ, MAP_PRIVATE, source_fd, 0);
    if (src_map == MAP_FAILED)
    {
        printf("ERROR: Failed to map source file: %s\n", strerror(errno));
        return FAILED_TO_MMAP_SOURCE_FILE;
    }

    char *dest_map = mmap(NULL, size_of_file_source, PROT_WRITE, MAP_SHARED, dest_fd, 0);
    if (dest_map == MAP_FAILED)
    {
        printf("ERROR: Failed to map destination file: %s\n", strerror(errno));
        return FAILED_TO_MMAP_DESTINATION_FILE;
    }

    printf("Copying %s to %s using mmap\n", src, dest);

    memcpy(dest_map, src_map, size_of_file_source);

    printf("Finished copying %s to %s using mmap\n", src, dest);

    if (munmap(src_map, size_of_file_source) < 0)
    {
        printf("ERROR: Failed to unmap source file: %s\n", strerror(errno));
        return FAILED_TO_MUNMAP_SOURCE_FILE;
    }

    if (munmap(dest_map, size_of_file_source) < 0)
    {
        printf("ERROR: Failed to unmap destination file: %s\n", strerror(errno));
        return FAILED_TO_MUNMAP_DESTINATION_FILE;
    }

    if (close(source_fd) < 0)
    {
        printf("ERROR: Failed to close source file: %s\n", strerror(errno));
        return FAILED_TO_CLOSE_FILE;
    }

    if (close(dest_fd) < 0)
    {
        printf("ERROR: Failed to close destination file: %s\n", strerror(errno));
        return FAILED_TO_CLOSE_FILE;
    }

    return SUCCESS;
}