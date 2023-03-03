#include <ctime>
#include <iostream>
#include <iomanip>
#include <sstream>

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "usage: " << argv[0] << "-[t | T | d | D | f | F  R]" << " <time string>" << std::endl;
        return EXIT_FAILURE;
    }

    std::tm t{};
    std::istringstream ss(argv[2]);

    ss >> std::get_time(&t, "%m-%d-%Y %H:%M:%S");

    if (ss.fail()) {
        std::cout << "failed to parse time string";
        return EXIT_FAILURE;
    }

    std::time_t time_stamp = mktime(&t);

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
            std::cout << "invalid format specifier" << std::endl;
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
