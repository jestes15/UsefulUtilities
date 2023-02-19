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
                    printf("  -h\t\tDisplay this help and exit\n");
                    printf("  -u\t\tUse the user space copy method\n");
                    printf("  -k\t\tUse the kernel space copy method\n");
                    printf("  -m\t\tUse the mmap copt method\n");
                    printf("  -f\t\tForce overwrite of DEST if it already exists\n");
                    printf("  -n\t\tDo not overwrite an existing file\n");
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
                    if (CHECK_FILE_PROTECTION(mode)) {
                        printf("ERROR: Illegal argument passed: %c\n", argv[argument][index]);
                        printf("Can not use option n with option m\n");
                        exit(ILLEGAL_OPTIONS_PASSED);
                    }
                    ENABLE_FORCE_WRITE(mode);
                    break;
                case 'n':
                    if (CHECK_FORCE_WRITE(mode)) {
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
        
        else if (!CHECK_SOURCE_BIT(path_set)) {
            source = argv[argument];
            SOURCE_BIT_SET(path_set);
        }

        else if (!CHECK_DEST_BIT(path_set)) {
            dest = argv[argument];
            DEST_BIT_SET(path_set);
        }
    }

    if (source == NULL) {
        printf("ERROR: Source path not provided\n");
        exit(SOURCE_PATH_NOT_VALID);
    }
    if (dest == NULL) {
        printf("ERROR: Destiniation path not provided\n");
        exit(NO_DESTINATION_PATH_PROVIDED);
    }

    if (CHECK_USER_SPACE_COPY_BIT(mode)) {
        printf("%d\n", faccessat(AT_FDCWD, source, F_OK | R_OK | W_OK, AT_SYMLINK_NOFOLLOW));
        printf("%d\n", faccessat(AT_FDCWD, dest, F_OK | R_OK | W_OK, AT_SYMLINK_NOFOLLOW));
        printf("Value of errno: %s\n", strerror(errno));
    }
}
