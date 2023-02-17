#include "copy.h"

// TODO: Add support for copying multiple files to a directory
int main(int argc, char *argv[])
{
    u_int8_t index;

    for (index = 1; index < argc; ++index)
    {
        if (argv[index][0] == '-')
        {
            switch (argv[index][1])
            {
            case 'h':
                printf("Usage: copy [OPTION] SOURCE DEST\n");
                printf("Copy SOURCE to DEST, or multiple SOURCE(s) to DIRECTORY.\n\n");
                printf("  -h\t\tDisplay this help and exit\n");
                printf("  -u\t\tUse the user space copy method\n");
                printf("  -k\t\tUse the kernel space copy method\n");
                printf("  -f\t\tForce overwrite of DEST if it already exists\n");
                printf("  -n\t\tDo not overwrite an existing file\n");
                exit(SUCCESS);
            case 'u':
                printf("Usage: copy [OPTION] SOURCE DEST\n");
                break;
            case 'k':
                printf("Usage: copy [OPTION] SOURCE DEST\n");
                break;
            case 'f':
                printf("Usage: copy [OPTION] SOURCE DEST\n");
                break;
            case 'n':
                printf("Usage: copy [OPTION] SOURCE DEST\n");
                break;
            default:
                printf("Usage: copy [OPTION] SOURCE DEST\n");
                break;
            }
        }
    }
}