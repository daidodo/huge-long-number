//#define TEST            //enable test
#define TEST_HUGE       //test Huge Number or POD
#define TEST_SIGNED     //test signed or unsigned
#define TEST_FILE       //use file compare or output directly

#ifndef __GNUC__
#   define __DZ_COMPILE_IN_VC8
#endif

#include <iostream>
#include "HugeLongNumber.h"

using namespace std;

#ifdef TEST
#   ifdef TEST_HUGE
#       ifdef TEST_SIGNED
typedef DoZerg::SignedHugeLong<4,short>     __TestType;
#       else
typedef DoZerg::UnsignedHugeLong<4,short>   __TestType;
#       endif
#   else
#       ifdef TEST_SIGNED
typedef long long                           __TestType;
#       else
typedef unsigned long long                  __TestType;
#       endif
#   endif
#   include <test\tests.h>
#else
const int TOTAL_BYTES = 1067;
typedef long Signed;
typedef DoZerg::UnsignedHugeLong<(TOTAL_BYTES + sizeof(Signed) - 1) / sizeof(Signed), Signed> Int;

#endif

int main()
{
#ifdef TEST
    test();
#else
    long N = 1000;
    Int r = 1;
    for(long i = 2;i <= N;++i)
        r *= i;
    cout<<N<<"! =\n"<<r<<endl;
#endif
}
