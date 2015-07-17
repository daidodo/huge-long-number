#include "huge_number.h"

#include <iostream>

using namespace std;

typedef HugeNumber Int;

#define ASSERT_EQ(a, b) assert_eq(a, b, __LINE__)

void assert_eq(const string & a, const Int & b, int line)
{
    if(a == b.toString())
        return;
    cerr<<"not equal at line "<<line<<":\na="<<a<<"\nb="<<b<<' '<<b.debugString()<<endl;
    exit(1);
}

void assert_eq(const char * a, const Int & b, int line)
{
    assert_eq(string(a), b, line);
}

template<typename T>
void assert_eq(const T & a, const Int & b, int line)
{
    ostringstream oss;
    if(sizeof(T) < sizeof(int))
        oss<<static_cast<int>(a);
    else
        oss<<a;
    assert_eq(oss.str(), b, line);
}

template<typename T>
void test_ctor_type()
{
    for(int i = -500;i <= 500;++i){
        const T ma = numeric_limits<T>::max() + i;
        const T mi = numeric_limits<T>::min() + i;
        const T z = i;
        Int a(ma), b(mi), c(z);
        ASSERT_EQ(ma, a);
        ASSERT_EQ(mi, b);
        ASSERT_EQ(z, c);
    }
}

void test_ctor()
{
    {
        Int a, b(a), c(std::move(a));
        //Int aa(true);
        //Int aa(1.0);
    }
    test_ctor_type<char>();
    test_ctor_type<wchar_t>();
    test_ctor_type<char16_t>();
    test_ctor_type<char32_t>();
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
    cout<<"test_ctor() SUCC\n";
}



int main()
{
    test_ctor();

    return 0;
}
