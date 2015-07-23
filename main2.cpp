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

void assert_eq(const string & a, const Int & b, int line, const char * func)
{
    if(a == b.toString())
        return;
    cerr << "not equal at line " << line << ":\na=" << a << "\nb=" << b << ' ' << b.debugString() << endl
        << "in " << func << endl;
    throw 1;
}

void assert_eq(const string & a, const string & b, int line, const char * func)
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
    }
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
    }
}

void test_compare_str_type(int cp, const Int & a, const string & s)
{
    constexpr int kSize = 200;
    char ss[kSize];
    const char (&css)[sizeof ss] = ss;
    strcpy(ss, s.c_str());
    __TEST_COMP(cp, a, s);
    __TEST_COMP(cp, a, s.c_str());
    __TEST_COMP(cp, a, ss);
    __TEST_COMP(cp, a, css);
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
    {
        const Int a(-1), b(0), c(1);
        __TEST_COMP(0, a, a);
        __TEST_COMP(0, b, b);
        __TEST_COMP(0, c, c);
        __TEST_COMP(-1, a, b);
        __TEST_COMP(-1, b, c);
        __TEST_COMP(-1, a, c);
        //__TEST_COMP(-1, b, true);
        //__TEST_COMP(-1, b, 1.);
    }
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

int main()
{
    test_ctor();
    test_sign();
    test_compare();
    test_bool();

    return 0;
}
