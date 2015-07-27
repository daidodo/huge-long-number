#include <iostream>
#include "huge_number.h"

using namespace std;

int main()
{
    HugeNumber a(1);
    for (int i = 2; i <= 1000; ++i)
        a *= i;
    cout << "1000! = \n"
        << a << endl;
    return 0;
}
