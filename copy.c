#include "copy.h"

// TODO: Add support for copying multiple files to a directory
// TODO: Add support for copying directories - https://github.com/coreutils/coreutils/blob/3edbf016be774e266a659349f513fe265c842e26/src/copy.c#L741
int main(int argc, char *argv[])
{
    u_int8_t argument, index = 1;
    char *source = NULL;
    char *dest = NULL;

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
                case 'v':
                    printf("copy version 1.0\n");
                    exit(SUCCESS);
                    break;
                case 't':
                    if (argument + 3 != argc)
                    {
                        printf("ERROR: Missing argument for option t\n");
                        exit(MISSING_ARGUMENT_FOR_OPTION);
                    }

                    copy_directory(argv[argument + 1], argv[argument + 2]);
                    exit(SUCCESS);
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
        printf("ERROR: %s\n", src);
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

info count_files(char *path)
{
    int count_files = 0;
    int count_directories = 0;
    struct dirent *entry;
    DIR *dir;
    if ((dir = opendir(path)) == NULL)
    {
        printf("ERROR: Failed to open directory: %s\n", strerror(errno));
        exit(FAILED_TO_OPEN_DIRECTORY);
    };

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_REG)
        {
            count_files++;
        }
        else if (entry->d_type == DT_DIR)
        {
            count_directories++;
        }
    }

    closedir(dir);

    info data = {
        .amount_of_files = count_files,
        .amount_of_directories = count_directories};

    return data;
}

files count_files_and_dirs(char *path)
{
    info data = count_files(path);
    struct dirent *entry;
    DIR *dir;
    char **file_names = malloc(sizeof(char *) * data.amount_of_files);
    char **directory_names = malloc(sizeof(char *) * data.amount_of_directories);

    if ((dir = opendir(path)) == NULL)
    {
        printf("ERROR: Failed to open directory: %s\n", strerror(errno));
        exit(FAILED_TO_OPEN_DIRECTORY);
    };

    u_int64_t file_index = 0;
    u_int64_t directory_index = 0;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_REG)
        {
            *(file_names + file_index) = entry->d_name;
            file_index++;
        }
        else if (entry->d_type == DT_DIR)
        {
            *(directory_names + directory_index) = entry->d_name;
            directory_index++;
        }
    }

    files path_data = {
        .file_list = file_names,
        .dir_list = directory_names,
        .file_count = data.amount_of_files,
        .dir_count = data.amount_of_directories};

    closedir(dir);
    return path_data;
}

int copy_directory(char *src, char *dest)
{
    files file_list = count_files_and_dirs(src);

    struct stat path_stat;
    if (stat(dest, &path_stat) == -1)
    {
        mkdir(dest, S_IRWXG | S_IRWXU);
    }

    for (int i = 0; i < file_list.dir_count; i++)
    {
        char *dir_name = *(file_list.dir_list + i);

        if (strcmp(dir_name, ".") == 0 || strcmp(dir_name, "..") == 0)
        {
            continue;
        }

        char *src_dir_path = malloc(strlen(src) + strlen(dir_name) + 2);
        char *dest_dir_path = malloc(strlen(dest) + strlen(dir_name) + 2);

        strcpy(src_dir_path, src);
        strcat(src_dir_path, "/");
        strcat(src_dir_path, dir_name);

        strcpy(dest_dir_path, dest);
        strcat(dest_dir_path, "/");
        strcat(dest_dir_path, dir_name);

        // printf("Copying directory %s to %s\n", src_dir_path, dest_dir_path);

        copy_directory(src_dir_path, dest_dir_path);

        free(src_dir_path);
        free(dest_dir_path);
    }

    for (int i = 0; i < file_list.file_count; i++)
    {
        char *file_name = *(file_list.file_list + i);

        struct stat path_stat;
        stat(dest, &path_stat);

        if (path_stat.st_mode & S_IFDIR) {
            continue;
        }

        char *src_file_path = malloc(strlen(src) + strlen(file_name) + 2);
        char *dest_file_path = malloc(strlen(dest) + strlen(file_name) + 2);

        strcpy(src_file_path, src);
        strcat(src_file_path, "/");
        strcat(src_file_path, file_name);

        strcpy(dest_file_path, dest);
        strcat(dest_file_path, "/");
        strcat(dest_file_path, file_name);

        printf("src file %s\n", src_file_path);
        printf("dest file %s\n", dest_file_path);

        // printf("Copying file %s to %s\n", src_file_path, dest_file_path);

        user_space_copy(src_file_path, dest_file_path);

        free(src_file_path);
        free(dest_file_path);
    }

    return 0;
}