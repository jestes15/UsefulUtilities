#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("usage: %s -[t | T | d | D | f | F  R] <month-day-year hour:minute:second>\n", argv[0]);
        return EXIT_FAILURE;
    }

    struct tm t;
if (!sscanf(argv[2], "%d-%d-%d %d:%d:%d", &t.tm_mon, &t.tm_mday, &t.tm_year, &t.tm_hour, &t.tm_min, &t.tm_sec)) {
    perror("Error: ");
    return EXIT_FAILURE;
}

// Adjust tm_year to start at 1900 and tm_mon to be in the range [0,11]
t.tm_year -= 1900;
t.tm_mon -= 1;

    time_t time_stamp = mktime(&t);

    switch (argv[1][1]) {
        case 't':
            printf("<t:%ld:t>\n", time_stamp);
            break;
        case 'T':
            printf("<t:%ld:T>\n", time_stamp);
            break;
        case 'd':
            printf("<t:%ld:d>\n", time_stamp);
            break;
        case 'D':
            printf("<t:%ld:D>\n", time_stamp);
            break;
        case 'f':
            printf("<t:%ld:f*>\n", time_stamp);
            break;
        case 'F':
            printf("<t:%ld:F>\n", time_stamp);
            break;
        case 'R':
            printf("<t:%ld:R>\n", time_stamp);
            break;
        default:
            printf("invalid format specifier");
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
