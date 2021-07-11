/*  see copyright notice in squirrel.h */
#ifndef _SQPCHEADER_H_
#define _SQPCHEADER_H_

#if defined(_MSC_VER) && defined(_DEBUG)
#include <crtdbg.h>
#endif

#if defined(_MSC_VER) 
#pragma warning(disable: 4706) // assignment within conditional expression 
#pragma warning(disable: 4611) // interaction between '_setjmp' and C++ object destruction is non-portable
#endif

#if defined(_MSC_VER) 
#define UNREACHABLE
#else
#define UNREACHABLE assert(0);
#endif

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <new>
//squirrel stuff
#include <squirrel.h>
#include "sqobject.h"
#include "sqstate.h"

#endif //_SQPCHEADER_H_
