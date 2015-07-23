#ifndef DOZERG_HUGE_NUMBER_20150715
#define DOZERG_HUGE_NUMBER_20150715

#include <cstdint>      //uint_fast64_t
#include <climits>      //CHAR_BIT
#include <utility>      //move
#include <vector>
#include <string>
#include <iostream>     //ostream
#include <sstream>      //ostringstream
#include <cassert>      //assert
#include <algorithm>    //reverse
#include <type_traits>  //is_integral, is_signed, make_signed_t, remove_cv_t

typedef uint_fast64_t               __Int;
typedef std::make_signed_t<__Int>   __SInt;

template<bool B, typename T1, typename T2>
struct __TypeSelect { typedef T2 type; };

template<typename T1, typename T2>
struct __TypeSelect<true, T1, T2> { typedef T1 type; };

template<typename T>
struct __SupportType{};

template<>struct __SupportType<std::string>{typedef const std::string & type;};
template<>struct __SupportType<const char *>{typedef const std::string & type;};
template<>struct __SupportType<char *>{typedef const std::string & type;};
//template<size_t N>struct __SupportType<const char [N]>{typedef const std::string & type;};
template<size_t N>struct __SupportType<char [N]>{typedef const std::string & type;};

#define __SUPPORT_INTEGER(tp)   \
    template<>struct __SupportType<tp>{ \
        typedef const typename __TypeSelect<std::is_signed<tp>::value, __SInt, __Int>::type & type; \
    }

__SUPPORT_INTEGER(char);
__SUPPORT_INTEGER(wchar_t);
__SUPPORT_INTEGER(char16_t);
__SUPPORT_INTEGER(char32_t);
__SUPPORT_INTEGER(signed char);
__SUPPORT_INTEGER(unsigned char);
__SUPPORT_INTEGER(short);
__SUPPORT_INTEGER(unsigned short);
__SUPPORT_INTEGER(int);
__SUPPORT_INTEGER(unsigned int);
__SUPPORT_INTEGER(long);
__SUPPORT_INTEGER(unsigned long);
__SUPPORT_INTEGER(long long);
__SUPPORT_INTEGER(unsigned long long);

#undef __SUPPORT_INTEGER

template<typename T>
using __SupportTypeT = typename __SupportType<T>::type;

template<class T, class F>
static void shrinkTailIf(T & c, F f)
{
    auto it = c.rbegin();
    if(it != c.rend() && f(*it)){
        for(++it;it != c.rend() && f(*it);++it);
        c.erase(it.base(), c.end());
    }
}

static void multWithTwo(std::string & s, int add)
{
    assert(0 <= add && add < 10);
    if(!s.empty())
        for(char & c : s){
            c = (c << 1) + add;
            if((add = (c > 9 ? 1 : 0)))
                c -= 10;
        }
    if(add)
        s.push_back(add);
}

static int divByTwo(std::string & s, bool & end)
{
    int c = 0;
    for(auto it = s.rbegin();it != s.rend();++it){
        *it += c * 10;
        c = (*it & 1);
        *it /= 2;
    }
    const auto p = s.find_last_not_of(char(0)) + 1;
    end = !p;
    s.erase(p);
    return c;
}

static void reverseAdd(std::string & s, int v)
{
    for(int i = 0, j = int(s.size() - 1);i <= j;++i, --j){
        const auto t = s[i];
        s[i] = s[j] + v;
        if(i < j)
            s[j] = t + v;
    }
}

static int unhex(char c)
{
    if('0' <= c && c <= '9')
        return (c - '0');
    if('a' <= c && c <= 'f')
        return (c - 'a' + 10);
    return (c - 'A' + 10);
}

//return:
//  2       base is 2
//  3       for "0", "-0", "+0", base is 10
//  8       base is 8
//  10      base is 10
//  16      base is 16
//  others  error
static int checkBase(const std::string & a)
{
    int r = 0;
    for(auto c : a){
        switch(r){
            case 0:
                if('+' == c || '-' == c)
                   break;
            case 1:
                if('0' == c){
                   r = 3;
                   break;
                }
            case 10:r = ('0' <= c && c <= '9' ? 10 : -1);break;
            case 3:
                if('b' == c || 'B' == c){
                    r = 5;
                    break;
                }else if('x' == c || 'X' == c){
                    r = 7;
                    break;
                }
            case 8:r = ('0' <= c && c < '8' ? 8 : -1);break;
            case 2:
            case 5:r = ('0' == c || '1' == c ? 2 : -1);break;
            case 7:
            case 16:r = (('0' <= c && c <= '9') || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F') ? 16 : -1);break;
        }
        if(r < 0)
            break;
    }
    return r;
}

template<typename T>
T getBits(const T & val, int pos, int bits)
{
    return (val >> pos) & ((T(1) << bits) - 1);
}

template<typename T>
void setBits(T & val, int pos, int bits, const T & v)
{
    const T m = (T(1) << bits) - 1;
    val &= ~(m << pos);
    val += (v & m) << pos;
}

template<class T>
class BitOp
{
    typedef typename T::value_type __Int;
    static constexpr int kEachBits = CHAR_BIT * sizeof(__Int);
    T & c_;
    int p_;
public:
    explicit BitOp(T & c):c_(c),p_(0){}
    void seek(int p){
        const int kTotalBits = int(kEachBits * c_.size());
        p_ = p;
        if(p_ > kTotalBits)
            p_ = kTotalBits;
        if(p_ < 0)
            p_ = 0;
    }
    void seekEnd(int p = 0){
        const int kTotalBits = int(kEachBits * c_.size());
        seek(p + kTotalBits);
    }
    bool read(int bits, __Int & val){
        if(!get(p_, bits, val))
            return false;
        seek(p_ + bits);
        return true;
    }
    bool readReverse(int bits, __Int & val){
        if(!get(p_ - bits, bits, val))
            return false;
        seek(p_ - bits);
        return true;
    }
    void write(int bits, const __Int & val) {
        const int kTotalBits = int(kEachBits * c_.size());
        if (kTotalBits < p_ + bits)
            c_.resize((p_ + bits + kEachBits - 1) / kEachBits);
        set(p_, bits, val);
        seek(p_ + bits);
    }
private:
    bool get(int p, int bits, __Int & val) const{
        assert(0 < bits && bits <= kEachBits);
        const int kTotalBits = int(kEachBits * c_.size());
        if(p < 0 || kTotalBits <= p)
            return false;
        const int fi = p / kEachBits, ri = p % kEachBits;
        val = getBits(c_[fi], ri, bits);
        if(kEachBits < ri + bits && size_t(fi + 1) < c_.size()){
            const int s1 = kEachBits - ri, s2 = bits - s1;
            setBits(val, s1, s2, getBits(c_[fi + 1], 0, s2));
        }
        return true;
    }
    bool set(int p, int bits, const __Int & val) {
        assert(0 < bits && bits <= kEachBits);
        const int kTotalBits = int(kEachBits * c_.size());
        if (p < 0 || kTotalBits <= p)
            return false;
        const int fi = p / kEachBits, ri = p % kEachBits;
        setBits(c_[fi], ri, bits, val);
        if (kEachBits < ri + bits && size_t(fi + 1) < c_.size()) {
            const int s1 = kEachBits - ri, s2 = bits - s1;
            setBits(c_[fi + 1], 0, s2, getBits(val, s1, s2));
        }
        return true;
    }
};

class HugeNumber
{
    //types
    typedef HugeNumber          __Myt;
    typedef std::vector<__Int>  __Data;
    //constants
    static constexpr int kEachBytes = sizeof(__Int);
    static constexpr int kEachBits = CHAR_BIT * kEachBytes;
public:
    //functions
    HugeNumber(){}  //Cannot be default for "const HugeNumber a;"
    HugeNumber(const __Myt & a) = default;
    HugeNumber(__Myt && a)
        : data_(std::move(a.data_))
        , sign_(a.sign_)
    {}
    template<typename T>
    explicit HugeNumber(const T & a){from(__SupportTypeT<T>(a));}
    __Myt & operator =(const __Myt & a) = default;
    __Myt && operator =(__Myt && a){
        if(&a != this){
            data_ = std::move(a.data_);
            sign_ = a.sign_;
        }
        return std::move(*this);
    }
    template<typename T>
    __Myt & operator =(const T & a){
        from(__SupportTypeT<T>(a));
        return *this;
    }
    //+a; -a;
    __Myt operator +(){return *this;}
    __Myt operator -(){
        __Myt t(*this);
        t.negate();
        return std::move(t);
    }
    //++a; --a;
    __Myt & operator ++() {
        *this += 1;
        return *this;
    }
    __Myt & operator --() {
        *this -= 1;
        return *this;
    }
    //a++; a--;
    __Myt operator ++(int){
        auto t(*this);
        ++*this;
        return std::move(t);
    }
    __Myt operator --(int){
        auto t(*this);
        --*this;
        return std::move(t);
    }
    //a += b;
    __Myt & operator +=(const __Myt & a) { add(a.sign_, a.data_); return *this; }
    template<typename T>
    __Myt & operator +=(const T & a) {
        add(__SupportTypeT<T>(a));
        return *this;
    }
    //a -= b;
    __Myt & operator -=(const __Myt & a){add(!a.sign_, a.data_);return *this;}
    template<typename T>
    __Myt & operator -=(const T & a) {
        sub(__SupportTypeT<T>(a));
        return *this;
    }
    //a *= b;
    __Myt & operator *=(const __Myt & a){mul(a.sign_, a.data_);return *this;}
    template<typename T>
    __Myt & operator *=(const T & a) {
        mul(__SupportTypeT<T>(a));
        return *this;
    }
    //a /= b;
    __Myt & operator /=(const __Myt & a){div(a.sign_, a.data_);return *this;}
    template<typename T>
    __Myt & operator /=(const T & a) {
        div(__SupportTypeT<T>(a));
        return *this;
    }
    //a %= b;
    __Myt & operator %=(const __Myt & a){mod(a.sign_, a.data_);return *this;}
    template<typename T>
    __Myt & operator %=(const T & a) {
        mod(__SupportTypeT<T>(a));
        return *this;
    }
    //a << 3; a >> 3;
    __Myt operator <<(int a) const{return (__Myt(*this) <<= a);}
    __Myt operator >>(int a) const{return (__Myt(*this) >>= a);}
    //a + b;
    __Myt operator +(const __Myt & a) const{return (__Myt(*this) += a);}
    template<class T>
    __Myt operator +(const T & a) const{return (__Myt(*this) += a);}
    template<class T>
    friend __Myt operator +(const T & a, const __Myt & b){return (b + a);}
    //a - b;
    __Myt operator -(const __Myt & a) const{return (__Myt(*this) -= a);}
    template<class T>
    __Myt operator -(const T & a) const{return (__Myt(*this) -= a);}
    template<class T>
    friend __Myt operator -(const T & a, const __Myt & b){
        auto t(b - a);
        t.negate();
        return std::move(t);
    }
    //a * b;
    template<class T>
    __Myt operator *(const T & a) const{return (__Myt(*this) *= a);}
    template<class T>
    friend __Myt operator *(const T & a, const __Myt & b){return (b * a);}
    //a / b;
    template<class T>
    __Myt operator /(const T & a) const{return (__Myt(*this) /= a);}
    template<class T>
    friend __Myt operator /(const T & a, const __Myt & b){return (__Myt(a) /= b);}
    //a % b;
    template<class T>
    __Myt operator %(const T & a) const{return (__Myt(*this) %= a);}
    template<class T>
    friend __Myt operator %(const T & a, const __Myt & b){return (__Myt(a) %= b);}
    //a <<= 3;
    __Myt & operator <<=(int a){
        if(a < 0)
            return (*this >>= (-a));
        if(!a || !*this)
            return *this;
        const int kTotalBits = int(kEachBits * data_.size());
        __Data r((kTotalBits + a + kEachBits - 1) / kEachBits);
        auto t = r.begin() + a / kEachBits, f = data_.begin();
        const int s1 = a % kEachBits;
        if(s1){
            const int s2 = kEachBits - s1;
            const __Int m1 = (__Int(1) << s2) - 1;
            const __Int m2 = (__Int(1) << s1) - 1;
            for(;f != data_.end();++f){
                *t++ += (*f & m1) << s1;
                *t = (*f >> s2) & m2;
            }
        }else
            std::copy(f, data_.end(), t);
        r.swap(data_);
        shrink();
        return *this;
    }
    //a >>= 3;
    __Myt & operator >>=(int a){
        if(a < 0)
            return (*this <<= (-a));
        if(!a)
            return *this;
        const int kTotalBits = int(kEachBits * data_.size());
        if(a < kTotalBits){
            auto t = data_.begin(), f = t + a / kEachBits;
            const int s1 = a % kEachBits;
            if(s1){
                const int s2 = kEachBits - s1;
                const __Int m1 = (__Int(1) << s2) - 1;
                const __Int m2 = (__Int(1) << s1) - 1;
                for(;f != data_.end();++t){
                    *t = (*f >> s1) & m1;
                    if(++f != data_.end())
                        *t += (*f & m2) << s2;
                }
                data_.erase(t, data_.end());
            }else
                data_.erase(t, f);
        }else
            data_.clear();
        shrink();
        return *this;
    }
    //bool(a); !a;
    explicit operator bool() const{return !operator !();}
    bool operator !() const{return data_.empty();}
    //a == b;
    bool operator ==(const __Myt & a) const{return (sign_ == a.sign_ && data_ == a.data_);}
    template<typename T>
    bool operator ==(const T & a) const{return equal(__SupportTypeT<T>(a));}
    template<typename T>
    friend bool operator ==(const T & a, const __Myt & b){return (b == a);}
    //a != b;
    bool operator !=(const __Myt & a) const{return !(*this == a);}
    template<class T>
    bool operator !=(const T & a) const{return !(*this == a);}
    template<class T>
    friend bool operator !=(const T & a, const __Myt & b){return (b != a);}
    //a < b;
    bool operator <(const __Myt & a) const{return less(a.sign_, a.data_);}
    template<typename T>
    bool operator <(const T & a) const{return less(__SupportTypeT<T>(a));}
    template<typename T>
    friend bool operator <(const T & a, const __Myt & b){return (b > a);}
    //a > b;
    bool operator >(const __Myt & a) const{return (a < *this);}
    template<typename T>
    bool operator >(const T & a) const{return greater(__SupportTypeT<T>(a));}
    template<typename T>
    friend bool operator >(const T & a, const __Myt & b){return (b < a);}
    //a <= b;
    bool operator <=(const __Myt & a) const{return !(a < *this);}
    template<class T>
    bool operator <=(const T & a) const{return !(a < *this);}
    template<typename T>
    friend bool operator <=(const T & a, const __Myt & b){return !(b < a);}
    //a >= b;
    bool operator >=(const __Myt & a) const{return !(*this < a);}
    template<class T>
    bool operator >=(const T & a) const{return !(*this < a);}
    template<typename T>
    friend bool operator >=(const T & a, const __Myt & b){return !(a < b);}
    //to string
    std::string toString(int base = 10, bool uppercase = false, bool showbase = false) const{
        const char * const kDigits = (uppercase ? "0123456789ABCDEF" : "0123456789abcdef");
        switch(base){
            case 16:return toStringX<4>(kDigits, (showbase ? (uppercase ? "X0" : "x0") : nullptr));
            case 10:return toString10();
            case 8:return toStringX<3>(kDigits, (showbase ? "0" : nullptr));
            case 2:return toStringX<1>(kDigits, (showbase ? (uppercase ? "B0" : "b0") : nullptr));
            default:;
        }
        assert(false && "Unsupported base");
        return std::string();
    }
    friend inline std::ostream & operator <<(std::ostream & os, const __Myt & a){
        const auto fmt = os.flags();
        const int base = ((fmt & os.hex) ? 16 : ((fmt & os.oct) ? 8 : 10));
        const bool uppercase = (0 != (fmt & os.uppercase));
        const bool showbase = (0 != (fmt & os.showbase));
        return (os<<a.toString(base, uppercase, showbase));
    }

    //debug only
    std::string debugString() const{
        std::ostringstream oss;
        oss<<'{'<<(sign_ ? '-' : '+')<<' ';
        for(auto v : data_)
            oss<<std::hex<<v<<' ';
        oss<<'}';
        return oss.str();
    }
private:
    void from(const __SInt & a){
        reset(a < 0);
        if(a)
            data_.push_back(abs(a));
    }
    void from(const __Int & a){
        reset();
        if(a)
            data_.push_back(a);
    }
    void from(const std::string & a){
        switch(checkBase(a)){
            case 2: {
                reset('-' == a[0]);
                BitOp<__Data> bits(data_);
                for (auto it = a.rbegin(); it != a.rend() && 'b' != *it && 'B' != *it; bits.write(1, *it++ - '0'));
                break; }
            case 3:reset();break;
            case 8: {
                reset('-' == a[0]);
                BitOp<__Data> bits(data_);
                for (auto it = a.rbegin(); it != a.rend() && '+' != *it && '-' != *it; bits.write(3, *it++ - '0'));
                break; }
            case 10:{
                reset('-' == a[0]);
                std::string t(a);
                if('+' == t[0] || '-' == t[0])
                    t[0] = '0';
                reverseAdd(t, -'0');
                BitOp<__Data> bits(data_);
                for(bool end = false;!end;bits.write(1, divByTwo(t, end)));
                break;}
            case 16:{
                reset('-' == a[0]);
                BitOp<__Data> bits(data_);
                for (auto it = a.rbegin(); it != a.rend() && 'x' != *it && 'X' != *it; bits.write(4, unhex(*it++)));
                break; }
            default:throw std::invalid_argument("Input is not a integer number");
        }
        shrink();
    }
    void negate(){
        if(*this)
            sign_ = !sign_;
    }
    void add(const __Int & a){add(false, a);}
    void add(const __SInt & a){add((a < 0), abs(a));}
    void add(const std::string & a){*this += __Myt(a);}
    template<class T>
    void add(bool s, const T & a){
        if(sign_ == s){
            addAbs(a);
            return;
        }
        switch(compare(a)){
            case 0:reset();break;
            case 1:subAbs(a);break;
            default:sign_ = s;subByAbs(a);
        }
    }
    void sub(const __Int & a) {add(true, a);}
    void sub(const __SInt & a) {add((a > 0), abs(a));}
    void sub(const std::string & a) {*this -= __Myt(a);}
    void mul(const __SInt & a) {}   //TODO
    void mul(const __Int & a) { }   //TODO
    void mul(const std::string & a) {*this *= __Myt(a);}
    void mul(bool s, const __Data & a){}    //TODO
    void div(const __SInt & a) {}   //TODO
    void div(const __Int & a) { }   //TODO
    void div(const std::string & a) {*this /= __Myt(a);}
    void div(bool s, const __Data & a){}    //TODO
    void mod(const __SInt & a) {}   //TODO
    void mod(const __Int & a) { }   //TODO
    void mod(const std::string & a) {*this %= __Myt(a);}
    void mod(bool s, const __Data & a){}    //TODO
    bool equal(const __SInt & a) const{return (sign_ == (a < 0) && 0 == compare(abs(a)));}
    bool equal(const __Int & a) const{return (!sign_ && 0 == compare(a));}
    bool equal(const std::string & a) const {return (*this == __Myt(a));}
    bool less(const __Int & a) const{return less(false, a);}
    bool less(const __SInt & a) const{return less((a < 0), abs(a));}
    bool less(const std::string & a) const{return (*this < __Myt(a));}
    template<class T>
    bool less(bool s, const T & a) const{
        if(sign_ != s)
            return sign_;
        const int r = compare(a);
        return (sign_ ? (1 == r) : (-1 == r));
    }
    bool greater(const __Int & a) const {return greater(false, a);}
    bool greater(const __SInt & a) const {return greater((a < 0), abs(a));}
    bool greater(const std::string & a) const {return (*this > __Myt(a));}
    bool greater(bool s, const __Int & a) const{
        if(sign_ != s)
            return !sign_;
        const int r = compare(a);
        return (sign_ ? (-1 == r) : (1 == r));
    }

    void reset(bool s = false){
        sign_ = s;
        data_.clear();
    }
    void shrink(){
        shrinkTailIf(data_, [](auto v){return (0 == v);});
        if(data_.empty() && sign_)
            sign_ = false;
    }
    template<int N>
    std::string toStringX(const char * digits, const char * base) const{
        static_assert(N > 0, "N is less than 1");
        std::string ret;
        if(data_.empty())
            ret.push_back('0');
        else{
            std::ostringstream oss;
            BitOp<const __Data> bits(data_);
            for(__Int i;bits.read(N, i);oss<<digits[i]);
            ret = oss.str();
            shrinkTailIf(ret, [](auto c){return ('0' == c);});
        }
        if(base)
            ret += base;
        if(sign_)
            ret.push_back('-');
        std::reverse(ret.begin(), ret.end());
        return std::move(ret);
    }
    std::string toString10() const{
        std::string ret;
        BitOp<const __Data> bits(data_);
        bits.seekEnd();
        for(__Int i;bits.readReverse(1, i);multWithTwo(ret, static_cast<int>(i)));
        if(ret.empty())
            ret.push_back(0);
        if(sign_)
            ret.push_back('-' - '0');
        reverseAdd(ret, '0');
        return std::move(ret);
    }
    int compare(const __Int & a) const{
        if(!a)
            return (data_.empty() ? 0 : 1);
        if(data_.size() < 1)
            return -1;
        else if(data_.size() > 1)
            return 1;
        return (data_[0] < a ? -1 : (data_[0] > a ? 1 : 0));
    }
    int compare(const __Data & a) const{
        if(data_.size() < a.size())
            return -1;
        else if(data_.size() > a.size())
            return 1;
        for(int i = int(a.size() - 1);i >= 0;--i)
            if(data_[i] < a[i])
                return -1;
            else if(data_[i] > a[i])
                return 1;
        return 0;
    }
    void addAbs(const __Int & a){(a ? addAbs({}, a) : (void)0);}
    void addAbs(const __Data & a, const __Int & b = 0){
        if(!b){
            if(a.empty())
                return;
            if(data_.empty()){
                data_ = a;
                return;
            }
        }
        __Int c = b;
        for(size_t i = 0;i < a.size() || c;++i){
            __Int t = c;
            c = 0;
            if(i < a.size())
                c += plus(t, a[i]);
            if(i < data_.size())
                c += plus(data_[i], t);
            else
                data_.push_back(t);
        }
    }
    void subAbs(const __Int & a){(a ? subAbs({}, a) : (void)0);}
    void subAbs(const __Data & a, const __Int & b = 0){
        if(a.empty() && !b)
            return;
        __Int c = b;
        for(size_t i = 0;i < a.size() || c;++i){
            c = minus(data_[i], c);
            if(i < a.size())
                c += minus(data_[i], a[i]);
        }
        shrink();
    }
    void subByAbs(const __Int & a){(a ? subByAbs({}, a) : (void)0);}
    void subByAbs(const __Data & a, const __Int & b = 0){
        if(data_.empty() && !b){
            data_ = a;
            return;
        }
        __Data r;
        __Int c = b, d = 0;
        for(size_t i = 0;i < data_.size() || c || d;++i){
            __Int t = c;
            c = 0;
            d = minus(t, d);
            if(i < a.size())
                c += plus(t, a[i]);
            if(i < data_.size())
                d += minus(t, data_[i]);
            if(c == d)
                c = d = 0;
            r.push_back(t);
        }
        r.swap(data_);
        shrink();
    }
    static __Int abs(const __SInt & a){
        return (a < 0 ? -a : a);
    }
    static int plus(__Int & a, const __Int & b){
        const auto t(a);
        a += b;
        return (a < t || a < b ? 1 : 0);
    }
    static int minus(__Int & a, const __Int & b){
        const int r = (a < b ? 1 : 0);
        a -= b;
        return r;
    }
    //fields
    __Data data_;
    bool sign_ = false;
};

#endif

