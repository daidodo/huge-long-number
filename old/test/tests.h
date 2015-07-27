/*
* Copyright (c) 2005 by DoZerg.  ALL RIGHTS RESERVED. 
* Consult your license regarding permissions and restrictions.
*/
#ifndef DOZERT_TEST_H_20080926
#define DOZERT_TEST_H_20080926

//#include "compare_file.h"
#ifdef __DZ_COMPILE_IN_VC8
#   ifdef TEST_SIGNED
const char * DZ_OUT_FILE = "result\\test_dz_signed_vc8.txt";
const char * SYS_OUT_FILE = "result\\test_sys_signed_vc8.txt";
#   else
const char * DZ_OUT_FILE = "result\\test_dz_unsigned_vc8.txt";
const char * SYS_OUT_FILE = "result\\test_sys_unsigned_vc8.txt";
#   endif
#else
#   ifdef TEST_SIGNED
const char * DZ_OUT_FILE = "result/test_dz_signed_gcc.txt";
const char * SYS_OUT_FILE = "result/test_sys_signed_gcc.txt";
#   else
const char * DZ_OUT_FILE = "result/test_dz_unsigned_gcc.txt";
const char * SYS_OUT_FILE = "result/test_sys_unsigned_gcc.txt";
#   endif
#endif

#ifdef TEST_HUGE
ofstream outf(DZ_OUT_FILE);
#else
ofstream outf(SYS_OUT_FILE);
#endif

#ifdef TEST_FILE
#   define cout outf
#endif

void Test001(){        //constructor,assignment,ToString,ostream <<,istream >>,ToSigned,hex,oct,dec,uppercase,showbase
    cout<<"\tTest001\n";
    const ios::fmtflags oldfmt = cout.flags();
    __TestType a0,a1(__Signed(-431)),a2(__Unsigned(-252));
    unsigned int ui = -3;
    __TestType b0(100),b1(int(-3)),b2 = ui;
    __TestType d(b0);
    a0 = -3;
#ifdef FOR_DZ
    __TestType c0("-200"),c1("200");
    cout<<"ToString: "<<a0.ToString()<<endl;
    cout<<"assignment: "<<(a0 = "1234")<<endl;
    cout<<"ToSigned & ToUnsigned: "<<a0.ToSigned()<<" "<<a0.ToUnsigned()<<endl;
    cout<<"Size: "<<a0.TOTAL_BYTES<<" "<<sizeof(a0)<<endl;
#else    //FOR_DZ
    cout<<"ToString: "<<a0<<endl;
    cout<<"assignment: "<<(a0 = 1234)<<endl;
    cout<<"ToSigned & ToUnsigned: "<<__Signed(a0)<<" "<<__Unsigned(a0)<<endl;
    cout<<"Size: "<<sizeof(a0)<<" "<<sizeof(a0)<<endl;
#endif    //FOR_DZ
    cout<<(a0 = d)<<endl;
#ifdef __DZ_COMPILE_IN_GCC
    std::istringstream ss("322");
#else //__DZ_COMPILE_IN_GCC
    std::istringstream ss("-322");
#endif //__DZ_COMPILE_IN_GCC
    ss>>a0;
    cout<<dec<<"DEC:";
    cout<<a0<<endl<<a1<<endl<<a2<<endl;
    cout<<b0<<endl<<b1<<endl<<b2<<endl;
    cout<<d<<endl;
    cout<<oct<<"OCT:";
    cout<<a0<<endl<<a1<<endl<<a2<<endl;
    cout<<b0<<endl<<b1<<endl<<b2<<endl;
    cout<<d<<endl;
    cout<<hex<<"HEX:";
    cout<<a0<<endl<<a1<<endl<<a2<<endl;
    cout<<b0<<endl<<b1<<endl<<b2<<endl;
    cout<<d<<endl;
    cout<<dec;

    __TestType i = 99,mi = -99;
    cout<<dec<<i<<" "<<mi<<endl;
    cout<<hex<<i<<" "<<mi<<endl;
    cout<<oct<<i<<" "<<mi<<endl;

    cout<<uppercase;
    cout<<dec<<i<<" "<<mi<<endl;
    cout<<hex<<i<<" "<<mi<<endl;
    cout<<oct<<i<<" "<<mi<<endl;

    cout<<showbase;
    cout<<dec<<i<<" "<<mi<<endl;
    cout<<hex<<i<<" "<<mi<<endl;
    cout<<oct<<i<<" "<<mi<<endl;

    cout.flags(oldfmt);
}
void Test002(){        //,=,+,-,~,<<,>>,++,--,>>=,<<=,abs
    cout<<"\tTest002\n";
    __TestType a(100),b(-100);
    cout<<(a = -100)<<endl;
#ifdef FOR_DZ
    cout<<(a = "200")<<endl;
#else    //FOR_DZ
    cout<<(a = 200)<<endl;
#endif    //FOR_DZ
    cout<<(a = b)<<endl;
    cout<<(+a)<<endl;
    cout<<(-a)<<endl;
    cout<<(~a)<<endl;
    cout<<(a<<2)<<endl;
    cout<<(a>>2)<<endl;
    cout<<(++a)<<endl;
    cout<<(a++)<<endl;
    cout<<(--a)<<endl;
    cout<<(a--)<<endl;
    cout<<(a<<=3)<<endl;
    cout<<(a>>=3)<<endl;
    cout<<(a = -1)<<endl;
    cout<<(++a)<<endl;
    cout<<(--a)<<endl;
#ifdef SIGNED
    cout<<"ABS:\n";
    cout<<abs(a)<<endl;
    cout<<abs(b)<<endl;
#endif //SIGNED
}
void Test003(){        //==,!=,<,>,>=,<=
#define TEST003
    cout<<"\tTest003"<<endl;
    __Signed av = 9,bv = 10,cv = 11;
#ifdef FOR_DZ
    char as[] = "9",bs[] = "10",cs[] = "11";
#else    //FOR_DZ
    __Signed as = 9,bs = 10,cs = 11;
#endif    //FOR_DZ
    __TestType a(10);
    int count = 0;
    cout<<(++count)<<'\t'<<(a != av ? "Yes" : "No")<<' '<<(a == av ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(a == bv ? "Yes" : "No")<<' '<<(a != bv ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(a != as ? "Yes" : "No")<<' '<<(a == as ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(a == bs ? "Yes" : "No")<<' '<<(a != bs ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(av != a ? "Yes" : "No")<<' '<<(av == a ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(bv == a ? "Yes" : "No")<<' '<<(bv != a ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(as != a ? "Yes" : "No")<<' '<<(as == a ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(bs == a ? "Yes" : "No")<<' '<<(bs != a ? "Yes" : "No")<<endl;
    __TestType b = 9,c = 10;
    cout<<(++count)<<'\t'<<(a != b ? "Yes" : "No")<<' '<<(a == b ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(a == c ? "Yes" : "No")<<' '<<(a != c ? "Yes" : "No")<<endl;

    cout<<(++count)<<'\t'<<(a >= av ? "Yes" : "No")<<' '<<(a < av ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(a >= bv ? "Yes" : "No")<<' '<<(a < bv ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(a < cv ? "Yes" : "No")<<' '<<(a >= cv ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(a >= as ? "Yes" : "No")<<' '<<(a < as ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(a >= bs ? "Yes" : "No")<<' '<<(a < bs ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(a < cs ? "Yes" : "No")<<' '<<(a >= cs ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(av < a ? "Yes" : "No")<<' '<<(av >= a ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(bv >= a ? "Yes" : "No")<<' '<<(bv < a ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(cv >= a ? "Yes" : "No")<<' '<<(cv < a ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(as < a ? "Yes" : "No")<<' '<<(as >= a ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(bs >= a ? "Yes" : "No")<<' '<<(bs < a ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(cs >= a ? "Yes" : "No")<<' '<<(cs < a ? "Yes" : "No")<<endl;
    __TestType d = 11;
    cout<<(++count)<<'\t'<<(a >= b ? "Yes" : "No")<<' '<<(a < b ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(a >= c ? "Yes" : "No")<<' '<<(a < c ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(a < d ? "Yes" : "No")<<' '<<(a >= d ? "Yes" : "No")<<endl;

    cout<<(++count)<<'\t'<<(a > av ? "Yes" : "No")<<' '<<(a <= av ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(a <= bv ? "Yes" : "No")<<' '<<(a > bv ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(a <= cv ? "Yes" : "No")<<' '<<(a > cv ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(a > as ? "Yes" : "No")<<' '<<(a <= as ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(a <= bs ? "Yes" : "No")<<' '<<(a > bs ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(a <= cs ? "Yes" : "No")<<' '<<(a > cs ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(av <= a ? "Yes" : "No")<<' '<<(av > a ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(bv <= a ? "Yes" : "No")<<' '<<(bv > a ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(cv > a ? "Yes" : "No")<<' '<<(cv <= a ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(as <= a ? "Yes" : "No")<<' '<<(as > a ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(bs <= a ? "Yes" : "No")<<' '<<(bs > a ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(cs > a ? "Yes" : "No")<<' '<<(cs <= a ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(a > b ? "Yes" : "No")<<' '<<(a <= b ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(a <= c ? "Yes" : "No")<<' '<<(a > c ? "Yes" : "No")<<endl;
    cout<<(++count)<<'\t'<<(a <= d ? "Yes" : "No")<<' '<<(a > d ? "Yes" : "No")<<endl;
}

void Test004(){        //+=,-=,*=,/=,%=,&=,|=,^=
    cout<<"\tTest004"<<endl;
    __TestType a(300),b(7);
    cout<<(a += 7)<<endl;
#ifdef FOR_DZ
    cout<<(a += "7")<<endl;
#else
    cout<<(a += 7)<<endl;
#endif
    cout<<(a += b)<<endl;
    cout<<(a -= 7)<<endl;
#ifdef FOR_DZ
    cout<<(a -= "7")<<endl;
#else
    cout<<(a -= 7)<<endl;
#endif
    cout<<(a -= b)<<endl;
    cout<<(a *= 7)<<endl;
#ifdef FOR_DZ
    cout<<(a *= "7")<<endl;
#else
    cout<<(a *= 7)<<endl;
#endif
    cout<<(a *= b)<<endl;
    cout<<"DIVIDE:"<<(a /= 7)<<endl;
#ifdef FOR_DZ
    cout<<(a /= "7")<<endl;
#else
    cout<<(a /= 7)<<endl;
#endif
    cout<<(a /= b)<<endl;
    cout<<(a %= 7)<<endl;
#ifdef FOR_DZ
    cout<<(a %= "7")<<endl;
#else
    cout<<(a %= 7)<<endl;
#endif
    cout<<(a %= b)<<endl;
    a = 100;b = 123;
    cout<<(a &= 7)<<endl;
#ifdef FOR_DZ
    cout<<(a &= "7")<<endl;
#else
    cout<<(a &= 7)<<endl;
#endif
    cout<<(a &= b)<<endl;
    cout<<(a |= 7)<<endl;
#ifdef FOR_DZ
    cout<<(a |= "7")<<endl;
#else
    cout<<(a |= 7)<<endl;
#endif
    cout<<(a |= b)<<endl;
    cout<<(a ^= 7)<<endl;
#ifdef FOR_DZ
    cout<<(a ^= "7")<<endl;
#else
    cout<<(a ^= 7)<<endl;
#endif
    cout<<(a ^= b)<<endl;
}
void Test005(){        //+,-,*,/,%,&,|,^
    cout<<"\tTest005"<<endl;
    __TestType a(50);
    cout<<"-------\t"<<a<<endl;
    cout<<"+\n";
    cout<<(a + 7)<<endl;
    cout<<(7 + a)<<endl;
    cout<<"-\n";
    cout<<(a - 7)<<endl;
    cout<<(7 - a)<<endl;
    cout<<"*\n";
    cout<<(a * 7)<<endl;
    cout<<(7 * a)<<endl;
    cout<<"/\n";
    cout<<(a / 7)<<endl;
    cout<<(7 / a)<<endl;
    cout<<"%\n";
    cout<<(a % 7)<<endl;
    cout<<(7 % a)<<endl;
    cout<<"&\n";
    cout<<(a & 7)<<endl;
    cout<<(7 & a)<<endl;
    cout<<"|\n";
    cout<<(a | 7)<<endl;
    cout<<(7 | a)<<endl;
    cout<<"^\n";
    cout<<(a ^ 7)<<endl;
    cout<<(7 ^ a)<<endl;
    cout<<"+-*/\n";
    cout<<(1 + 2 * (3 - a) / 4)<<endl;

    a = -10;
    cout<<"-------\t"<<a<<endl;
    cout<<"+\n";
    cout<<(a + 7)<<endl;
    cout<<(7 + a)<<endl;
    cout<<"-\n";
    cout<<(a - 7)<<endl;
    cout<<(7 - a)<<endl;
    cout<<"*\n";
    cout<<(a * 7)<<endl;
    cout<<(7 * a)<<endl;
    cout<<"/\n";
    cout<<(a / 7)<<endl;
    cout<<(7 / a)<<endl;
    cout<<"%\n";
    cout<<(a % 7)<<endl;
    cout<<(7 % a)<<endl;
    cout<<"&\n";
    cout<<(a & 7)<<endl;
    cout<<(7 & a)<<endl;
    cout<<"|\n";
    cout<<(a | 7)<<endl;
    cout<<(7 | a)<<endl;
    cout<<"^\n";
    cout<<(a ^ 7)<<endl;
    cout<<(7 ^ a)<<endl;
    cout<<"+-*/\n";
    cout<<(1 + 2 * (3 - a) / 4)<<endl;

    a = -5;
    cout<<"-------\t"<<a<<endl;
    cout<<"+\n";
    cout<<(a + 7)<<endl;
    cout<<(7 + a)<<endl;
    cout<<"-\n";
    cout<<(a - 7)<<endl;
    cout<<(7 - a)<<endl;
    cout<<"*\n";
    cout<<(a * 7)<<endl;
    cout<<(7 * a)<<endl;
    cout<<"/\n";
    cout<<(a / 7)<<endl;
    cout<<(7 / a)<<endl;
    cout<<"%\n";
    cout<<(a % 7)<<endl;
    cout<<(7 % a)<<endl;
    cout<<"&\n";
    cout<<(a & 7)<<endl;
    cout<<(7 & a)<<endl;
    cout<<"|\n";
    cout<<(a | 7)<<endl;
    cout<<(7 | a)<<endl;
    cout<<"^\n";
    cout<<(a ^ 7)<<endl;
    cout<<(7 ^ a)<<endl;
    cout<<"+-*/\n";
    cout<<(1 + 2 * (3 - a) / 4)<<endl;

    a = -7;
    cout<<"-------\t"<<a<<endl;
    cout<<"+\n";
    cout<<(a + 7)<<endl;
    cout<<(7 + a)<<endl;
    cout<<"-\n";
    cout<<(a - 7)<<endl;
    cout<<(7 - a)<<endl;
    cout<<"*\n";
    cout<<(a * 7)<<endl;
    cout<<(7 * a)<<endl;
    cout<<"/\n";
    cout<<(a / 7)<<endl;
    cout<<(7 / a)<<endl;
    cout<<"%\n";
    cout<<(a % 7)<<endl;
    cout<<(7 % a)<<endl;
    cout<<"&\n";
    cout<<(a & 7)<<endl;
    cout<<(7 & a)<<endl;
    cout<<"|\n";
    cout<<(a | 7)<<endl;
    cout<<(7 | a)<<endl;
    cout<<"^\n";
    cout<<(a ^ 7)<<endl;
    cout<<(7 ^ a)<<endl;
    cout<<"+-*/\n";
    cout<<(1 + 2 * (3 - a) / 4)<<endl;

    cout<<"string\n";
#ifdef FOR_DZ
    cout<<(a + "7")<<endl;
    cout<<("7" + a)<<endl;
    cout<<(a - "7")<<endl;
    cout<<("7" - a)<<endl;
    cout<<(a * "7")<<endl;
    cout<<("7" * a)<<endl;
    cout<<(a / "7")<<endl;
    cout<<("7" / a)<<endl;
    cout<<(a % "7")<<endl;
    cout<<("7" % a)<<endl;
    cout<<(a & "7")<<endl;
    cout<<("7" & a)<<endl;
    cout<<(a | "7")<<endl;
    cout<<("7" | a)<<endl;
    cout<<(a ^ "7")<<endl;
    cout<<("7" ^ a)<<endl;
#else
    cout<<(a + 7)<<endl;
    cout<<(7 + a)<<endl;
    cout<<(a - 7)<<endl;
    cout<<(7 - a)<<endl;
    cout<<(a * 7)<<endl;
    cout<<(7 * a)<<endl;
    cout<<(a / 7)<<endl;
    cout<<(7 / a)<<endl;
    cout<<(a % 7)<<endl;
    cout<<(7 % a)<<endl;
    cout<<(a & 7)<<endl;
    cout<<(7 & a)<<endl;
    cout<<(a | 7)<<endl;
    cout<<(7 | a)<<endl;
    cout<<(a ^ 7)<<endl;
    cout<<(7 ^ a)<<endl;
#endif
    __TestType b;
    a = 3;b = 5;
    cout<<"a = "<<a<<"\tb = "<<b<<endl;
    cout<<(a + b)<<endl;
    cout<<(a - b)<<endl;
    cout<<(a * b)<<endl;
    cout<<(a / b)<<endl;
    cout<<(a % b)<<endl;
    cout<<(a & b)<<endl;
    cout<<(a | b)<<endl;
    cout<<(a ^ b)<<endl;
    a = -3;b = 5;
    cout<<"a = "<<a<<"\tb = "<<b<<endl;
    cout<<(a + b)<<endl;
    cout<<(a - b)<<endl;
    cout<<(a * b)<<endl;
    cout<<(a / b)<<endl;
    cout<<(a % b)<<endl;
    cout<<(a & b)<<endl;
    cout<<(a | b)<<endl;
    cout<<(a ^ b)<<endl;
    a = -3;b = -5;
    cout<<"a = "<<a<<"\tb = "<<b<<endl;
    cout<<(a + b)<<endl;
    cout<<(a - b)<<endl;
    cout<<(a * b)<<endl;
    cout<<(a / b)<<endl;
    cout<<(a % b)<<endl;
    cout<<(a & b)<<endl;
    cout<<(a | b)<<endl;
    cout<<(a ^ b)<<endl;
    a = 3;b = -5;
    cout<<"a = "<<a<<"\tb = "<<b<<endl;
    cout<<(a + b)<<endl;
    cout<<(a - b)<<endl;
    cout<<(a * b)<<endl;
    cout<<(a / b)<<endl;
    cout<<(a % b)<<endl;
    cout<<(a & b)<<endl;
    cout<<(a | b)<<endl;
    cout<<(a ^ b)<<endl;

    b = -3;a = 5;
    cout<<"a = "<<a<<"\tb = "<<b<<endl;
    cout<<(a + b)<<endl;
    cout<<(a - b)<<endl;
    cout<<(a * b)<<endl;
    cout<<(a / b)<<endl;
    cout<<(a % b)<<endl;
    cout<<(a & b)<<endl;
    cout<<(a | b)<<endl;
    cout<<(a ^ b)<<endl;
    b = -3;a = -5;
    cout<<"a = "<<a<<"\tb = "<<b<<endl;
    cout<<(a + b)<<endl;
    cout<<(a - b)<<endl;
    cout<<(a * b)<<endl;
    cout<<(a / b)<<endl;
    cout<<(a % b)<<endl;
    cout<<(a & b)<<endl;
    cout<<(a | b)<<endl;
    cout<<(a ^ b)<<endl;
    b = 3;a = -5;
    cout<<"a = "<<a<<"\tb = "<<b<<endl;
    cout<<(a + b)<<endl;
    cout<<(a - b)<<endl;
    cout<<(a * b)<<endl;
    cout<<(a / b)<<endl;
    cout<<(a % b)<<endl;
    cout<<(a & b)<<endl;
    cout<<(a | b)<<endl;
    cout<<(a ^ b)<<endl;

    a = 5;b = -5;
    cout<<"a = "<<a<<"\tb = "<<b<<endl;
    cout<<(a + b)<<endl;
    cout<<(a - b)<<endl;
    cout<<(a * b)<<endl;
    cout<<(a / b)<<endl;
    cout<<(a % b)<<endl;
    cout<<(a & b)<<endl;
    cout<<(a | b)<<endl;
    cout<<(a ^ b)<<endl;
    a = -5;b = 5;
    cout<<"a = "<<a<<"\tb = "<<b<<endl;
    cout<<(a + b)<<endl;
    cout<<(a - b)<<endl;
    cout<<(a * b)<<endl;
    cout<<(a / b)<<endl;
    cout<<(a % b)<<endl;
    cout<<(a & b)<<endl;
    cout<<(a | b)<<endl;
    cout<<(a ^ b)<<endl;
    a = 0;b = 5;
    cout<<"a = "<<a<<"\tb = "<<b<<endl;
    cout<<(a + b)<<endl;
    cout<<(a - b)<<endl;
    cout<<(a * b)<<endl;
    cout<<(a / b)<<endl;
    cout<<(a % b)<<endl;
    cout<<(a & b)<<endl;
    cout<<(a | b)<<endl;
    cout<<(a ^ b)<<endl;
}

#ifdef CROSS_TEST
typedef __TestSigned    __I_TestType1;
typedef __TestUnsigned    __I_TestType2;

void I_Test001(){        //==,!=,<,>,<=,>=
    cout<<"\tI_Test001"<<endl;
    __I_TestType1 b(20);
    __I_TestType2 a(b);
    a = b + 10;
    __I_TestType2 c(a),d;
    d = a + 10;
    cout<<(a == b ? "Yes" : "No")<<endl;
    cout<<(a == c ? "Yes" : "No")<<endl;
    cout<<(b == a ? "Yes" : "No")<<endl;
    cout<<(c == a ? "Yes" : "No")<<endl;
    cout<<(a != b ? "Yes" : "No")<<endl;
    cout<<(a != c ? "Yes" : "No")<<endl;
    cout<<(b != a ? "Yes" : "No")<<endl;
    cout<<(c != a ? "Yes" : "No")<<endl;
    cout<<(a < b ? "Yes" : "No")<<endl;
    cout<<(a < c ? "Yes" : "No")<<endl;
    cout<<(a < d ? "Yes" : "No")<<endl;
    cout<<(b < a ? "Yes" : "No")<<endl;
    cout<<(c < a ? "Yes" : "No")<<endl;
    cout<<(d < a ? "Yes" : "No")<<endl;
    cout<<(a > b ? "Yes" : "No")<<endl;
    cout<<(a > c ? "Yes" : "No")<<endl;
    cout<<(a > d ? "Yes" : "No")<<endl;
    cout<<(b > a ? "Yes" : "No")<<endl;
    cout<<(c > a ? "Yes" : "No")<<endl;
    cout<<(d > a ? "Yes" : "No")<<endl;
    cout<<(a <= b ? "Yes" : "No")<<endl;
    cout<<(a <= c ? "Yes" : "No")<<endl;
    cout<<(a <= d ? "Yes" : "No")<<endl;
    cout<<(b <= a ? "Yes" : "No")<<endl;
    cout<<(c <= a ? "Yes" : "No")<<endl;
    cout<<(d <= a ? "Yes" : "No")<<endl;
    cout<<(a >= b ? "Yes" : "No")<<endl;
    cout<<(a >= c ? "Yes" : "No")<<endl;
    cout<<(a >= d ? "Yes" : "No")<<endl;
    cout<<(b >= a ? "Yes" : "No")<<endl;
    cout<<(c >= a ? "Yes" : "No")<<endl;
    cout<<(d >= a ? "Yes" : "No")<<endl;
}

#endif    //CROSS_TEST

void test(){
    Test001();
    //Test002();
    //Test003();
    //Test004();
    //Test005();
}
