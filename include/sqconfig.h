#define __STDC_FORMAT_MACROS // Linux/Adnroid won't define PRId* macros without this
#include <stdint.h>
#include <inttypes.h>

#ifdef _SQ64
    typedef int64_t  SQInteger;
    typedef uint64_t SQUnsignedInteger;
    typedef uint64_t SQHash; /*should be the same size of a pointer*/
#else
    typedef intptr_t SQInteger;
    typedef uintptr_t SQUnsignedInteger;
    typedef uintptr_t SQHash; /*should be the same size of a pointer*/
#endif

typedef int SQInt32;
typedef unsigned int SQUnsignedInteger32;


#ifndef __forceinline
#define __forceinline inline
#endif

#ifdef SQUSEDOUBLE
    typedef double SQFloat;
#else
    typedef float SQFloat;
#endif

#if defined(SQUSEDOUBLE) && !defined(_SQ64) || !defined(SQUSEDOUBLE) && defined(_SQ64)
    typedef int64_t SQRawObjectVal; //must be 64bits
    #define SQ_OBJECT_RAWINIT() { _unVal.raw = 0; }
#else
    typedef SQUnsignedInteger SQRawObjectVal; //is 32 bits on 32 bits builds and 64 bits otherwise
    #define SQ_OBJECT_RAWINIT()
#endif

#ifndef SQ_ALIGNMENT // SQ_ALIGNMENT shall be less than or equal to SQ_MALLOC alignments, and its value shall be power of 2.
    #if defined(SQUSEDOUBLE) || defined(_SQ64)
        #define SQ_ALIGNMENT 8
    #else
        #define SQ_ALIGNMENT 4
    #endif
#endif

typedef void* SQUserPointer;
typedef SQUnsignedInteger SQBool;
typedef SQInteger SQRESULT;

#ifdef SQUNICODE
#include <wchar.h>
#include <wctype.h>


typedef wchar_t SQChar;


#define scstrcmp    wcscmp
#define scstrncmp   wcsncmp
#ifdef _WIN32
#define scsprintf   _snwprintf
#else
#define scsprintf   swprintf
#endif
#define scstrlen    wcslen
#define scstrtod    wcstod
#ifdef _SQ64
#define scstrtol    wcstoll
#else
#define scstrtol    wcstol
#endif
#define scstrtoul   wcstoul
#define scvsprintf  vswprintf
#define scstrstr    wcsstr
#define scprintf    wprintf
#define scstrrchr   wcsrchr

#ifdef _WIN32
#define WCHAR_SIZE 2
#define WCHAR_SHIFT_MUL 1
#define MAX_CHAR 0xFFFF
#else
#define WCHAR_SIZE 4
#define WCHAR_SHIFT_MUL 2
#define MAX_CHAR 0xFFFFFFFF
#endif

#define _SC(a) L##a


#define scisspace   iswspace
#define scisdigit   iswdigit
#define scisprint   iswprint
#define scisxdigit  iswxdigit
#define scisalpha   iswalpha
#define sciscntrl   iswcntrl
#define scisalnum   iswalnum


#define sq_rsl(l) ((l)<<WCHAR_SHIFT_MUL)

#else // SQUNICODE

typedef char SQChar;
#define _SC(a) a
#define scstrcmp    strcmp
#define scstrncmp   strncmp
#ifdef _MSC_VER
#define scsprintf   _snprintf
#else
#define scsprintf   snprintf
#endif
#define scstrlen    strlen
#define scstrtod    strtod
#ifdef _SQ64
#ifdef _MSC_VER
#define scstrtol    _strtoi64
#else
#define scstrtol    strtoll
#endif
#else
#define scstrtol    strtol
#endif
#define scstrtoul   strtoul
#define scvsprintf  vsnprintf
#define scstrstr    strstr
#define scstrrchr   strrchr
#define scisspace   isspace
#define scisdigit   isdigit
#define scisprint   isprint
#define scisxdigit  isxdigit
#define sciscntrl   iscntrl
#define scisalpha   isalpha
#define scisalnum   isalnum
#define scprintf    printf
#define MAX_CHAR 0xFF

#define sq_rsl(l) (l)

#endif // SQUNICODE

#ifdef _SQ64
    #define _PRINT_INT_PREC _SC("ll")
    #define _PRINT_INT_FMT _SC("%" PRId64)
#else
    #define _PRINT_INT_FMT _SC("%d")
#endif
#define SQ_CHECK_THREAD_LEVEL_NONE 0
#define SQ_CHECK_THREAD_LEVEL_FAST 1
#define SQ_CHECK_THREAD_LEVEL_DEEP 2

#ifndef SQ_CHECK_THREAD
#define SQ_CHECK_THREAD SQ_CHECK_THREAD_LEVEL_NONE
#endif
