#pragma once

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
typedef long long llong;
typedef unsigned long long ullong;

typedef ullong uint64;
typedef uint uint32;
typedef ushort uint16;
typedef uchar uint8;

typedef llong int64;
typedef int int32;
typedef short int16;
typedef char int8;

typedef uchar *rawptr_t;

#ifdef _WIN64
typedef uint64 nuint;
#else
typedef uint32 nuint;
#endif

typedef uchar char8;
typedef ushort char16;
typedef uint char32;

#ifdef _WIN64
typedef uint64 nuint;
#else
typedef uint32 nuint;
#endif
