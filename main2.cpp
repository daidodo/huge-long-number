#include "huge_number.h"

#include <iostream>
#include <random>

using namespace std;

typedef HugeNumber Int;

#define ASSERT_EQ(a, b)     assert_eq(a, b, __LINE__, typeid(a))

void assert_eq(const string & a, const Int & b, int line, const std::type_info & type)
{
    if(a == b.toString())
        return;
    cerr << "not equal at line " << line << ":\na=" << a << "\nb=" << b << ' ' << b.debugString() << endl
        << "for " << type.name() << endl;
    exit(1);
}

void assert_eq(const string & a, const string & b, int line, const std::type_info & type)
{
    if(a == b)
        return;
    cerr << "not equal at line " << line << ":\na=" << a << "\nb=" << b << endl
        << "for " << type.name() << endl;
    exit(1);
}

void assert_eq(const char * a, const Int & b, int line, const std::type_info & type)
{
    assert_eq(string(a), b, line, type);
}

template<typename T>
void assert_eq(const T & a, const Int & b, int line, const std::type_info & type)
{
    ostringstream oss;
    if(sizeof(T) < sizeof(int))
        oss<<static_cast<int>(a);
    else
        oss<<a;
    assert_eq(oss.str(), b, line, type);
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
    string s(bstr);
    s.push_back(genChar(head));
    genArray(s, 200, body);
    {
        Int a(s), b('+' + s), aa, bb;
        ASSERT_EQ(s, a.toString(base, upper, true));
        ASSERT_EQ(s, b.toString(base, upper, true));
        aa = s;
        bb = '+' + s;
        ASSERT_EQ(s, aa.toString(base, upper, true));
        ASSERT_EQ(s, bb.toString(base, upper, true));
    }{
        s = '-' + s;
        Int a(s), aa;
        ASSERT_EQ(s, a.toString(base, upper, true));
        aa = s;
        ASSERT_EQ(s, aa.toString(base, upper, true));
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
        Int a, b(a), c(move(a));
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
    cout<<"test_ctor() SUCC\n";
}

void test_add()
{
    Int a;
    //a += "123";
    a += 123;
}



int main()
{
    test_ctor();

    return 0;
}
