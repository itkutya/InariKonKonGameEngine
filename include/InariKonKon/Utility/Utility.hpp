#ifndef IKK_UTILITY_HPP
#define IKK_UTILITY_HPP

#include <cstdint>

#ifdef NDEBUG
    #define IKK_RELEASE 1
#else
    #define IKK_DEBUG 1
#endif

#define U64(x)      static_cast<std::uint64_t>(x)
#define U32(x)      static_cast<std::uint32_t>(x)
#define U16(x)      static_cast<std::uint16_t>(x)
#define U8(x)       static_cast<std::uint8_t>(x)

#define I64(x)      static_cast<std::int64_t>(x)
#define I32(x)      static_cast<std::int32_t>(x)
#define I16(x)      static_cast<std::int16_t>(x)
#define I8(x)       static_cast<std::int8_t>(x)

#define INT(x)      static_cast<std::int32_t>(x)
#define BOOL(x)     static_cast<bool>(x)
#define FLOAT(x)    static_cast<float>(x) 

#define TO_UTF8(x)  reinterpret_cast<const char8_t*>(x)
#define TO_ANSI(x)  reinterpret_cast<const char*>(x)

#endif