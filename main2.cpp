#include "huge_number.h"

#include <iostream>
#include <random>
#include <cstring>

using namespace std;

typedef HugeNumber Int;

#ifdef WIN32
#   define __PRETTY_FUNCTION__  __FUNCSIG__
#endif

#define ASSERT_EQ(a, b)     assert_eq(a, b, __LINE__, __PRETTY_FUNCTION__)

void assert_eq(string a, string b, int line, const char * func)
{
    if(a == b)
        return;
    cerr << "not equal at line " << line << ":\na=" << a << "\nb=" << b << endl
        << "in " << func << endl;
    throw 1;
}

void assert_eq(bool a, bool b, int line, const char * func)
{
    if(a == b)
        return;
    cerr << "not equal at line " << line << ":\na=" << boolalpha<< a << "\nb=" << b << endl
        << "in " << func << endl;
    throw 1;
}

void assert_eq(string a, const Int & b, int line, const char * func)
{
    if(a == b.toString())
        return;
    cerr << "not equal at line " << line << ":\na=" << a << "\nb=" << b << ' ' << b.debugString() << endl
        << "in " << func << endl;
    throw 1;
}

void assert_eq(const Int & a, const Int & b, int line, const char * func)
{
    if(a == b)
        return;
    cerr << "not equal at line " << line << ":\na=" << a << "\nb=" << b << endl
        << "in " << func << endl;
    throw 1;
}

void assert_eq(const char * a, const Int & b, int line, const char * func)
{
    assert_eq(string(a), b, line, func);
}

template<typename T>
void assert_eq(const T & a, const Int & b, int line, const char * func)
{
    ostringstream oss;
    if(sizeof(T) < sizeof(int))
        oss<<static_cast<int>(a);
    else
        oss<<a;
    assert_eq(oss.str(), b, line, func);
}

static default_random_engine gen;

int genRand(int from, int to)
{
    uniform_int_distribution<int> u(from, to);
    return u(gen);
}

char genChar(string s)
{
    assert(!s.empty());
    return s[genRand(0, int(s.size() - 1))];
}

template<class T>
void genArray(T & a, size_t len, typename T::value_type from, typename T::value_type to)
{
    uniform_int_distribution<decltype(from)> u(from, to);
    for(size_t i = 0;i < len;++i, a.push_back(u(gen)));
}

template<class T>
void genArray(T & a, size_t len, const T & b)
{
    assert(!b.empty());
    uniform_int_distribution<decltype(b.size())> u(0, b.size() - 1);
    for(size_t i = 0;i < len;++i, a.push_back(b[u(gen)]));
}

template<typename T>
void test_ctor_type()
{
    for(int i = -100;i <= 100;++i){
        const T ma = numeric_limits<T>::max() + i;
        const T mi = numeric_limits<T>::min() + i;
        const T z = i;
        Int a(ma), b(mi), c(z), aa, bb, cc;
        ASSERT_EQ(ma, a);
        ASSERT_EQ(mi, b);
        ASSERT_EQ(z, c);
        aa = ma;
        bb = mi;
        cc = z;
        ASSERT_EQ(ma, aa);
        ASSERT_EQ(mi, bb);
        ASSERT_EQ(z, cc);
        aa = aa;
        bb = bb;
        cc = cc;
        ASSERT_EQ(ma, aa);
        ASSERT_EQ(mi, bb);
        ASSERT_EQ(z, cc);
    }
}

void test_ctor_str_base_case(int base, bool upper, string bstr, string head, string body)
{
    constexpr int kSize = 200;
    string s(bstr);
    s.push_back(genChar(head));
    genArray(s, kSize, body);
    {
        char ss[kSize + 16];
        const char (&css)[sizeof ss] = ss;
        {
            strcpy(ss, s.c_str());
            Int a(s), b(s.c_str()), c(ss), d(css), aa, bb, cc, dd;
            ASSERT_EQ(s, a.toString(base, upper, true));
            ASSERT_EQ(s, b.toString(base, upper, true));
            ASSERT_EQ(s, c.toString(base, upper, true));
            ASSERT_EQ(s, d.toString(base, upper, true));
            aa = s;
            bb = s.c_str();
            cc = ss;
            dd = css;
            ASSERT_EQ(s, aa.toString(base, upper, true));
            ASSERT_EQ(s, bb.toString(base, upper, true));
            ASSERT_EQ(s, cc.toString(base, upper, true));
            ASSERT_EQ(s, dd.toString(base, upper, true));
        }{
            string t('+' + s);
            strcpy(ss, t.c_str());
            Int a(s), b(s.c_str()), c(ss), d(css), aa, bb, cc, dd;
            ASSERT_EQ(s, a.toString(base, upper, true));
            ASSERT_EQ(s, b.toString(base, upper, true));
            ASSERT_EQ(s, c.toString(base, upper, true));
            ASSERT_EQ(s, d.toString(base, upper, true));
            aa = s;
            bb = s.c_str();
            cc = ss;
            dd = css;
            ASSERT_EQ(s, aa.toString(base, upper, true));
            ASSERT_EQ(s, bb.toString(base, upper, true));
            ASSERT_EQ(s, cc.toString(base, upper, true));
            ASSERT_EQ(s, dd.toString(base, upper, true));
        }
        s.insert(s.begin(), '-');
        {
            strcpy(ss, s.c_str());
            Int a(s), b(s.c_str()), c(ss), d(css), aa, bb, cc, dd;
            ASSERT_EQ(s, a.toString(base, upper, true));
            ASSERT_EQ(s, b.toString(base, upper, true));
            ASSERT_EQ(s, c.toString(base, upper, true));
            ASSERT_EQ(s, d.toString(base, upper, true));
            aa = s;
            bb = s.c_str();
            cc = ss;
            dd = css;
            ASSERT_EQ(s, aa.toString(base, upper, true));
            ASSERT_EQ(s, bb.toString(base, upper, true));
            ASSERT_EQ(s, cc.toString(base, upper, true));
            ASSERT_EQ(s, dd.toString(base, upper, true));
        }
    }
}

template<class T>
void test_ctor_others(T s)
{
    const Int a(s);
}

void test_ctor_str()
{
    test_ctor_str_base_case(2, false, "0b", "1", "01");
    test_ctor_str_base_case(2, true, "0B", "1", "01");
    test_ctor_str_base_case(8, false, "0", "1234567", "01234567");
    test_ctor_str_base_case(8, true, "0", "1234567", "01234567");
    test_ctor_str_base_case(10, false, "", "123456789", "0123456789");
    test_ctor_str_base_case(10, true, "", "123456789", "0123456789");
    test_ctor_str_base_case(16, false, "0x", "123456789abcdef", "0123456789abcdef");
    test_ctor_str_base_case(16, true, "0X", "123456789ABCDEF", "0123456789ABCDEF");
    {
        char s[124];
        strcpy(s, "124");
        const char ss[123] = "123";
        const Int a(s), b(ss), c("123");
        test_ctor_others(s);    //Strange!!
        test_ctor_others(ss);
    }
}

void test_ctor()
{
    {
        Int a, b(a), c(move(a)), d(static_cast<const Int &&>(a));
        const Int e;
        a = b;
        b = move(c);
        //Int aa(true);
        //Int bb(1.0);
        //a = true;
        //a = 1.;
    }
    test_ctor_type<char>();
    test_ctor_type<wchar_t>();
    test_ctor_type<char16_t>();
#ifndef WIN32   // char32_t in Visual Studio 2015 RC acts strangely
    test_ctor_type<char32_t>();
#endif
    test_ctor_type<signed char>();
    test_ctor_type<unsigned char>();
    test_ctor_type<short>();
    test_ctor_type<unsigned short>();
    test_ctor_type<int>();
    test_ctor_type<unsigned int>();
    test_ctor_type<long>();
    test_ctor_type<unsigned long>();
    test_ctor_type<long long>();
    test_ctor_type<unsigned long long>();
    test_ctor_str();
    cout<<__FUNCTION__<<"() SUCC\n";
}

void test_sign()
{
    {   //positive
        Int a;
        a = 123;
        ASSERT_EQ("123", +a);
        a = 0;
        ASSERT_EQ("0", +a);
        a = -0;
        ASSERT_EQ("0", +a);
        a = -123;
        ASSERT_EQ("-123", +a);
    }{  //negate
        Int a;
        a = 123;
        ASSERT_EQ("-123", -a);
        a = 0;
        ASSERT_EQ("0", -a);
        a = -0;
        ASSERT_EQ("0", -a);
        a = -123;
        ASSERT_EQ("123", -a);
    }
    cout<<__FUNCTION__<<"() SUCC\n";
}

template<class T, class S>
static int comp(const T & a, const S & b)
{
    return (a == b ? 0 : (a < b ? -1 : 1));
}

#define __TEST_COMP(cp, a, b)   do{ \
    ASSERT_EQ(0 == cp, a == b); \
    ASSERT_EQ(0 == cp, b == a);  \
    ASSERT_EQ(0 != cp, a != b); \
    ASSERT_EQ(0 != cp, b != a);  \
    ASSERT_EQ(-1 == cp, a < b); \
    ASSERT_EQ(1 == cp, b < a);  \
    ASSERT_EQ(1 == cp, a > b);  \
    ASSERT_EQ(-1 == cp, b > a); \
    ASSERT_EQ(1 != cp, a <= b); \
    ASSERT_EQ(-1 != cp, b <= a);  \
    ASSERT_EQ(-1 != cp, a >= b); \
    ASSERT_EQ(1 != cp, b >= a);  \
}while(0)

template<typename T>
void test_compare_type()
{
    for(int i = -100;i <= 100;++i){
        const T ma = numeric_limits<T>::max() + i;
        const T mi = numeric_limits<T>::min() + i;
        const T z = i;
        const Int a(ma), b(mi), c(z);
        for(int j = -10;j <= 10;++j){
            const T maj(ma + j), mij(mi + j), zj(z + j);
            __TEST_COMP(comp(ma, maj), a, maj);
            __TEST_COMP(comp(mi, mij), b, mij);
            __TEST_COMP(comp(z, zj), c, zj);
        }
        __TEST_COMP(0, a, a);
        __TEST_COMP(0, b, b);
        __TEST_COMP(0, c, c);
    }
}

void test_compare_str_type(int cp, const Int & a, string s)
{
    constexpr int kSize = 200;
    char ss[kSize];
    const char (&css)[sizeof ss] = ss;
    strcpy(ss, s.c_str());
    const Int b(s);
    __TEST_COMP(cp, a, s);
    __TEST_COMP(cp, a, s.c_str());
    __TEST_COMP(cp, a, ss);
    __TEST_COMP(cp, a, css);
    __TEST_COMP(cp, a, b);
    __TEST_COMP(0, a, a);
}

void test_compare_str()
{
    test_compare_str_type(1, Int("2415134134141341302394292428"), "0");
    test_compare_str_type(0, Int(), "0");
    test_compare_str_type(-1, Int("-2415134134141341302394292428"), "0");

    test_compare_str_type(1, Int("2415134134141341302394292428"), "123");
    test_compare_str_type(-1, Int(), "123");
    test_compare_str_type(-1, Int("-2415134134141341302394292428"), "123");

    test_compare_str_type(1, Int("2415134134141341302394292429"), "2415134134141341302394292428");
    test_compare_str_type(0, Int("2415134134141341302394292428"), "2415134134141341302394292428");
    test_compare_str_type(-1, Int("2415134134141341302394292427"), "2415134134141341302394292428");
    test_compare_str_type(-1, Int(), "2415134134141341302394292428");
    test_compare_str_type(-1, Int("-2415134134141341302394292427"), "2415134134141341302394292428");
    test_compare_str_type(-1, Int("-2415134134141341302394292428"), "2415134134141341302394292428");
    test_compare_str_type(-1, Int("-2415134134141341302394292429"), "2415134134141341302394292428");

    test_compare_str_type(1, Int("2415134134141341302394292428"), "-123");
    test_compare_str_type(1, Int(), "-123");
    test_compare_str_type(-1, Int("-2415134134141341302394292428"), "-123");

    test_compare_str_type(1, Int("2415134134141341302394292429"), "-2415134134141341302394292428");
    test_compare_str_type(1, Int("2415134134141341302394292428"), "-2415134134141341302394292428");
    test_compare_str_type(1, Int("2415134134141341302394292427"), "-2415134134141341302394292428");
    test_compare_str_type(1, Int(), "-2415134134141341302394292428");
    test_compare_str_type(1, Int("-2415134134141341302394292427"), "-2415134134141341302394292428");
    test_compare_str_type(0, Int("-2415134134141341302394292428"), "-2415134134141341302394292428");
    test_compare_str_type(-1, Int("-2415134134141341302394292429"), "-2415134134141341302394292428");
}

void test_compare()
{
    //__TEST_COMP(-1, Int(), true);
    //__TEST_COMP(-1, Int(), 1.);
    test_compare_type<char>();
    test_compare_type<wchar_t>();
    test_compare_type<char16_t>();
    test_compare_type<char32_t>();
    test_compare_type<signed char>();
    test_compare_type<unsigned char>();
    test_compare_type<short>();
    test_compare_type<unsigned short>();
    test_compare_type<int>();
    test_compare_type<unsigned int>();
    test_compare_type<long>();
    test_compare_type<unsigned long>();
    test_compare_type<long long>();
    test_compare_type<unsigned long long>();
    test_compare_str();
    cout<<__FUNCTION__<<"() SUCC\n";
}

#undef __TEST_COMP

void test_bool()
{
    const Int a;
    const Int b("12345"), c("-12345");
    const Int d("1304165134617364173013561034"), e("-1304165134617364173013561034");
    ASSERT_EQ(false, (a ? true : false));
    ASSERT_EQ(true, !a);
    ASSERT_EQ(true, (b ? true : false));
    ASSERT_EQ(false, !b);
    ASSERT_EQ(true, (c ? true : false));
    ASSERT_EQ(false, !c);
    ASSERT_EQ(true, (d ? true : false));
    ASSERT_EQ(false, !d);
    ASSERT_EQ(true, (e ? true : false));
    ASSERT_EQ(false, !e);
    cout<<__FUNCTION__<<"() SUCC\n";
}

void test_incr_decr_exp(string s, string ss)
{
    Int a(s), b(s);
    ASSERT_EQ(ss, ++a);
    ASSERT_EQ(s, b++);
    ASSERT_EQ(ss, b);
    ASSERT_EQ(s, --a);
    ASSERT_EQ(ss, b--);
    ASSERT_EQ(s, b);
}

void test_incr_decr()
{
    test_incr_decr_exp("-14910398570183471326419783246138", "-14910398570183471326419783246137");
    test_incr_decr_exp("-18446744073709551616", "-18446744073709551615");
    test_incr_decr_exp("-18446744073709551615", "-18446744073709551614");
    test_incr_decr_exp("-9223372036854775809", "-9223372036854775808");
    test_incr_decr_exp("-9223372036854775808", "-9223372036854775807");
    test_incr_decr_exp("-123", "-122");
    test_incr_decr_exp("-1", "0");
    test_incr_decr_exp("0", "1");
    test_incr_decr_exp("123", "124");
    test_incr_decr_exp("9223372036854775806", "9223372036854775807");
    test_incr_decr_exp("9223372036854775807", "9223372036854775808");
    test_incr_decr_exp("18446744073709551614", "18446744073709551615");
    test_incr_decr_exp("18446744073709551615", "18446744073709551616");
    test_incr_decr_exp("14910398570183471326419783246138", "14910398570183471326419783246139");
    cout<<__FUNCTION__<<"() SUCC\n";
}

#define __TEST_ADD_SUB(s1, s2, ss)  do{ \
    const Int a(s1), b(s2), c(ss);  \
    ASSERT_EQ(c, Int(s1) += b);     \
    ASSERT_EQ(c, Int(s1) += s2);    \
    ASSERT_EQ(c, Int(s2) += a);     \
    ASSERT_EQ(c, Int(s2) += s1);    \
    ASSERT_EQ(a, Int(ss) -= b);     \
    ASSERT_EQ(a, Int(ss) -= s2);    \
    ASSERT_EQ(b, Int(ss) -= a);     \
    ASSERT_EQ(b, Int(ss) -= s1);    \
    ASSERT_EQ(c, a + b);            \
    ASSERT_EQ(c, a + (s2));         \
    ASSERT_EQ(c, (s1) + b);         \
    ASSERT_EQ(a, c - b);            \
    ASSERT_EQ(a, c - (s2));         \
    ASSERT_EQ(a, (ss) - b);         \
    ASSERT_EQ(b, c - a);            \
    ASSERT_EQ(b, c - (s1));         \
    ASSERT_EQ(b, (ss) - a);         \
}while(0)

#define __TEST_ADD_SUB2(s, ss)  do{ \
    const Int a(s), b(ss);  \
    Int v(s);               \
    ASSERT_EQ(b, v += v);   \
    ASSERT_EQ("0", v -= v); \
    ASSERT_EQ(b, a + a);    \
    ASSERT_EQ("0", a - a);  \
}while(0)

template<typename T>
void test_add_sub_type()
{
    const T ma = numeric_limits<T>::max();
    const T mi = numeric_limits<T>::min();
    const T z = 0;
    for(T i = 0;i <= 10;++i){
        __TEST_ADD_SUB(ma - i, i, ma);
        __TEST_ADD_SUB(mi, i, mi + i);
        __TEST_ADD_SUB(z, i, i);
        __TEST_ADD_SUB2(z, z);
        __TEST_ADD_SUB2(i, 2 * i);
        if(mi < 0){
            __TEST_ADD_SUB(-i, i, z);
            __TEST_ADD_SUB2(-i, -2 * i);
        }
    }
}

void test_add_sub_str_exp(string s, string ss)
{
    constexpr int kSize = 200;
    char cs[kSize], css[kSize];
    const char (&ccs)[kSize] = cs;
    const char (&ccss)[kSize] = css;
    strcpy(cs, s.c_str());
    strcpy(css, ss.c_str());
    __TEST_ADD_SUB2(s, ss);
    __TEST_ADD_SUB2(cs, css);
    __TEST_ADD_SUB2(ccs, ccss);
}

void test_add_sub_str_exp(string s1, string s2, string ss)
{
    constexpr int kSize = 200;
    char cs1[kSize], cs2[kSize], css[kSize];
    const char (&ccs1)[kSize] = cs1;
    const char (&ccs2)[kSize] = cs2;
    const char (&ccss)[kSize] = css;
    strcpy(cs1, s1.c_str());
    strcpy(cs2, s2.c_str());
    strcpy(css, ss.c_str());
    __TEST_ADD_SUB(s1, s2, ss);
    __TEST_ADD_SUB(cs1, cs2, css);
    __TEST_ADD_SUB(ccs1, ccs2, ccss);
}

#undef __TEST_ADD_SUB
#undef __TEST_ADD_SUB2

void test_add_sub_str()
{
    {
        test_add_sub_str_exp("-4444444444444444444444444444444444444",
                             "-8888888888888888888888888888888888888");
        test_add_sub_str_exp("-15000000000000000000",
                             "-30000000000000000000");
        test_add_sub_str_exp("-123", "-246");
        test_add_sub_str_exp("0", "0");
        test_add_sub_str_exp("123", "246");
        test_add_sub_str_exp("15000000000000000000",
                             "30000000000000000000");
        test_add_sub_str_exp("4444444444444444444444444444444444444",
                             "8888888888888888888888888888888888888");
    }{
        test_add_sub_str_exp(
                "-4444444444444444444444444444444444444",
                "-6666666666666666666666666666666666666",
               "-11111111111111111111111111111111111110");
        test_add_sub_str_exp(
                "-4444444444444444444444444444444444444",
                                 "-15000000000000000000",
                "-4444444444444444459444444444444444444");
        test_add_sub_str_exp(
                "-4444444444444444444444444444444444444",
                                                  "-123",
                "-4444444444444444444444444444444444567");
        test_add_sub_str_exp(
                "-4444444444444444444444444444444444444",
                                                     "0",
                "-4444444444444444444444444444444444444");
        test_add_sub_str_exp(
                "-4444444444444444444444444444444444444",
                                                   "123",
                "-4444444444444444444444444444444444321");
        test_add_sub_str_exp(
                "-4444444444444444444444444444444444444",
                                  "14000000000000000000",
                "-4444444444444444430444444444444444444");
        test_add_sub_str_exp(
                "-4444444444444444444444444444444444444",
                 "4444444444444444444444444444444444440",
                                                    "-4");
        test_add_sub_str_exp(
                "-4444444444444444444444444444444444444",
                 "4444444444444444444444444444444444444",
                                                     "0");
        test_add_sub_str_exp(
                "-4444444444444444444444444444444444444",
                 "4444444444444444444444444444444444448",
                                                     "4");
        test_add_sub_str_exp(
                "-4444444444444444444444444444444444444",
                 "6666666666666666666666666666666666666",
                 "2222222222222222222222222222222222222");
    }{
        test_add_sub_str_exp(
                "-15000000000000000000",
                "-16000000000000000000",
                "-31000000000000000000");
        test_add_sub_str_exp(
                "-15000000000000000000",
                                 "-123",
                "-15000000000000000123");
        test_add_sub_str_exp(
                "-15000000000000000000",
                                    "0",
                "-15000000000000000000");
        test_add_sub_str_exp(
                "-15000000000000000000",
                                  "123",
                "-14999999999999999877");
        test_add_sub_str_exp(
                "-15000000000000000000",
                 "14999999999999990000",
                               "-10000");
        test_add_sub_str_exp(
                "-15000000000000000000",
                 "15000000000000000000",
                                    "0");
        test_add_sub_str_exp(
                "-15000000000000000000",
                 "15000000000000100000",
                               "100000");
        test_add_sub_str_exp(
                                 "-15000000000000000000",
                 "6666666666666666666666666666666666666",
                 "6666666666666666651666666666666666666");
    }{
        test_add_sub_str_exp(
                "-123",
                "-234",
                "-357");
        test_add_sub_str_exp(
                "-123",
                "0",
                "-123");
        test_add_sub_str_exp(
                "-123",
                 "122",
                  "-1");
        test_add_sub_str_exp(
                "-123",
                 "123",
                   "0");
        test_add_sub_str_exp(
                "-123",
                 "124",
                   "1");
        test_add_sub_str_exp(
                                "-123",
                "15000000000000000000",
                "14999999999999999877");
        test_add_sub_str_exp(
                                                 "-123",
                "6666666666666666666666666666666666666",
                "6666666666666666666666666666666666543");
    }{
        test_add_sub_str_exp(
                "0",
                "0",
                "0");
        test_add_sub_str_exp(
                  "0",
                "123",
                "123");
        test_add_sub_str_exp(
                                   "0",
                "15000000000000000000",
                "15000000000000000000");
        test_add_sub_str_exp(
                                                    "0",
                "6666666666666666666666666666666666666",
                "6666666666666666666666666666666666666");
    }{
        test_add_sub_str_exp(
                "123",
                "234",
                "357");
        test_add_sub_str_exp(
                                 "123",
                "15000000000000000000",
                "15000000000000000123");
        test_add_sub_str_exp(
                                                  "123",
                "6666666666666666666666666666666666666",
                "6666666666666666666666666666666666789");
    }{
        test_add_sub_str_exp(
                "15000000000000000000",
                "16000000000000000000",
                "31000000000000000000");
        test_add_sub_str_exp(
                                 "15000000000000000000",
                "6666666666666666666666666666666666666",
                "6666666666666666681666666666666666666");
    }{
        test_add_sub_str_exp(
                "3333333333333333333333333333333333333",
                "6666666666666666666666666666666666666",
                "9999999999999999999999999999999999999");
    }
}

void test_add_sub()
{
    test_add_sub_type<char>();
    test_add_sub_type<wchar_t>();
    test_add_sub_type<char16_t>();
    test_add_sub_type<char32_t>();
    test_add_sub_type<signed char>();
    test_add_sub_type<unsigned char>();
    test_add_sub_type<short>();
    test_add_sub_type<unsigned short>();
    test_add_sub_type<int>();
    test_add_sub_type<unsigned int>();
    test_add_sub_type<long>();
    test_add_sub_type<unsigned long>();
    test_add_sub_type<long long>();
    test_add_sub_type<unsigned long long>();
    test_add_sub_str();
    cout<<__FUNCTION__<<"() SUCC\n";
}

void test_shift()
{
    cout<<__FUNCTION__<<"() SUCC\n";
}

int main()
{
    test_ctor();
    test_sign();
    test_compare();
    test_bool();
    test_incr_decr();
    test_add_sub();
    test_shift();

    return 0;
}
