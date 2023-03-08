#include <Poco/Format.h>
#include <iostream>

typedef float f32;
typedef double f64;

typedef int8_t i8;
typedef u8 u8;

typedef int16_t i16;
typedef uint16_t u16;

typedef int32_t i32;
typedef u32 u32;

typedef int64_t i64;
typedef uint64_t u64;

i32
main() {
    std::cout << Poco::format("%Ld", static_cast<i64>(23874)) << std::endl;
	std::cout << sizeof(unsigned long) << " " << sizeof(u32) << " " << sizeof(uint64_t) << std::endl; 
    return 0;
}