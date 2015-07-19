#ifndef DOZERG_HUGE_NUMBER_20150715
#define DOZERG_HUGE_NUMBER_20150715

#include <cstdint>  //uint_fast64_t
#include <climits>  //CHAR_BIT
#include <utility>  //move
#include <vector>
#include <string>
#include <iostream> //ostream
#include <sstream>  //ostringstream
#include <cassert>  //assert
#include <algorithm>    //reverse
#include <type_traits>  //is_integral, is_signed, make_signed_t

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

static void reverseAdd(std::string & s, int v)
{
    for(int i = 0, j = s.size() - 1;i <= j;++i, --j){
        const auto t = s[i];
        s[i] = s[j] + v;
        if(i < j)
            s[j] = t + v;
    }
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
        const int kTotalBits = kEachBits * c_.size();
        p_ = p;
        if(p_ > kTotalBits)
            p_ = kTotalBits;
        if(p_ < 0)
            p_ = 0;
    }
    void seekEnd(int p = 0){
        const int kTotalBits = kEachBits * c_.size();
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
        const int kTotalBits = kEachBits * c_.size();
        if (kTotalBits < p_ + bits)
            c_.resize((p_ + bits + kEachBits - 1) / kEachBits);
        set(p_, bits, val);
        seek(p_ + bits);
    }
private:
    bool get(int p, int bits, __Int & val) const{
        assert(0 < bits && bits <= kEachBits);
        const int kTotalBits = kEachBits * c_.size();
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
        const int kTotalBits = kEachBits * c_.size();
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
    typedef uint_fast64_t       __Int;
    typedef std::vector<__Int>  __Data;
    //constants
    static constexpr int kEachBytes = sizeof(__Int);
    static constexpr int kEachBits = CHAR_BIT * kEachBytes;
public:
    //functions
    HugeNumber() = default;
    HugeNumber(const __Myt & a) = default;
    HugeNumber(__Myt && a):data_(std::move(a.data_)){}
    template<typename T>
    explicit HugeNumber(const T & a){fromInteger(a, typename std::is_integral<T>::type());}
    explicit HugeNumber(const std::string & a){fromString(a);}
    __Myt & operator =(const __Myt & a) = default;
    __Myt && operator =(__Myt && a){
        if(&a != this)
            data_ = std::move(a.data_);
        return std::move(*this);
    }
    template<typename T>
    __Myt & operator =(const T & a){
        fromInteger(a, typename std::is_integral<T>::type());
        return *this;
    }
    __Myt & operator =(const std::string & a){
        fromString(a);
        return *this;
    }
    __Myt & operator ++();  //TODO
    __Myt & operator --();  //TODO
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
    __Myt operator +(){return *this;}
    __Myt operator -(){
        __Myt t(*this);
        t.sign_ = !sign_;
        return std::move(t);
    }
    __Myt & operator <<=(int a){
        if(a < 0)
            return (*this >>= (-a));
        if(!a || !*this)
            return *this;
        const int kTotalBits = kEachBits * data_.size();
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
    __Myt & operator >>=(int a){
        if(a < 0)
            return (*this <<= (-a));
        if(!a)
            return *this;
        const int kTotalBits = kEachBits * data_.size();
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
    __Myt & operator +=(const __Myt & a){addSignData(a.sign_, a.data_);return *this;}
    __Myt & operator -=(const __Myt & a){addSignData(!a.sign_, a.data_);return *this;}
    __Myt & operator *=(const __Myt & a);   //TODO
    __Myt & operator /=(const __Myt & a);   //TODO
    __Myt & operator %=(const __Myt & a);   //TODO
    __Myt operator <<(int a) const{return __Myt(*this).operator <<=(a);}
    __Myt operator >>(int a) const{return __Myt(*this).operator >>=(a);}
    __Myt operator +(const __Myt & a) const{return __Myt(*this).operator +=(a);}
    __Myt operator -(const __Myt & a) const{return __Myt(*this).operator -=(a);}
    __Myt operator *(const __Myt & a) const{return __Myt(*this).operator *=(a);}
    __Myt operator /(const __Myt & a) const{return __Myt(*this).operator /=(a);}
    __Myt operator %(const __Myt & a) const{return __Myt(*this).operator %=(a);}
    explicit operator bool() const{return !operator !();}
    bool operator !() const{return data_.empty();}
    bool operator ==(const __Myt & a) const{return (sign_ == a.sign_ && data_ == a.data_);}
    bool operator !=(const __Myt & a) const{return !operator ==(a);}
    bool operator <(const __Myt & a) const{
        if(sign_ != a.sign_)
            return sign_;
        return (compareData(a.data_) ? !sign_ : sign_);
    }
    bool operator >(const __Myt & a) const{return a.operator <(*this);}
    bool operator <=(const __Myt & a) const{return !a.operator <(*this);}
    bool operator >=(const __Myt & a) const{return !operator <(a);}

    std::string toString(int base = 10, bool uppercase = false, bool showbase = false) const{
        const char * const kDigits = (uppercase ? "0123456789ABCDEF" : "0123456789abcdef");
        switch(base){
            case 16:return bitsToString<4>(kDigits, (showbase ? (uppercase ? "X0" : "x0") : nullptr));
            case 10:return tenToString();
            case 8:return bitsToString<3>(kDigits, (showbase ? "0" : nullptr));
            case 2:return bitsToString<1>(kDigits, (showbase ? (uppercase ? "B0" : "b0") : nullptr));
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
        oss<<'{'<<std::boolalpha<<sign_<<' ';
        for(auto v : data_)
            oss<<std::hex<<v<<' ';
        oss<<'}';
        return oss.str();
    }
private:
    explicit HugeNumber(bool) = delete;
    template<typename T>
    void fromInteger(const T & a, std::true_type){
        sign_ = (a < 0);
        if(a){
            data_.resize(1);
            if(std::is_signed<T>::value){
                std::make_signed_t<__Int> t(a);
                data_.back() = (sign_ ? -t : t);
            }else
                data_.back() = a;
        }else
            data_.clear();
    }
    void fromString(const std::string & a){ //TODO
        switch(checkBase(a)){
            case 2: {
                data_.clear();
                BitOp<__Data> bits(data_);
                for (auto it = a.rbegin(); it != a.rend() && 'b' != *it && 'B' != *it; bits.write(1, *it++ - '0'));
                break; }
            case 3:data_.clear(); break;
            case 8: {
                data_.clear();
                BitOp<__Data> bits(data_);
                for (auto it = a.rbegin(); it != a.rend() && '+' != *it && '-' != *it; bits.write(3, *it++ - '0'));
                break; }
            case 10:
            case 16:
            default:throw std::invalid_argument("Input is not a integer number");
        }
    }
    void shrink(){
        shrinkTailIf(data_, [](auto v){return (0 == v);});
        if(data_.empty() && sign_)
            sign_ = false;
    }
    template<int N>
    std::string bitsToString(const char * digits, const char * base) const{
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
    std::string tenToString() const{
        std::string ret;
        BitOp<const __Data> bits(data_);
        bits.seekEnd();
        for(__Int i;bits.readReverse(1, i);multWithTwo(ret, static_cast<int>(i)));
        //for(auto it = data_.rbegin();it != data_.rend();++it)
        //    for(int i = kEachBits - 1;i >= 0;--i)
        //        multWithTwo(ret, ((*it >> i) & 1));
        if(ret.empty())
            ret.push_back(0);
        if(sign_)
            ret.push_back('-' - '0');
        reverseAdd(ret, '0');
        return std::move(ret);
    }
    bool compareData(const __Data & a) const{
        if(data_.size() != a.size())
            return (data_.size() < a.size());
        for(int i = a.size() - 1;i >= 0;--i)
            if(data_[i] != a[i])
                return (data_[i] < a[i]);
        return false;
    }
    void addSignData(bool s, const __Data & a){
        if(sign_ == s)
            addData(a);
        else if(compareData(a)){
            sign_ = s;
            subByData(a);
        }else
            subData(a);
    }
    void addData(const __Data & a){
        if(a.empty())
            return;
        if(data_.empty()){
            data_ = a;
            return;
        }
        int c = 0;
        for(size_t i = 0;i < a.size() || c;++i){
            __Int t = c;
            if(i < a.size())
                t += a[i];
            if(i < data_.size())
                data_[i] += t;
            else
                data_.push_back(t);
            c = (t < a[i] || data_[i] < t ? 1 : 0);
        }
    }
    void subData(const __Data & a){
        if(a.empty())
            return;
        int c = 0;
        for(size_t i = 0;i < a.size() || c;++i){
            __Int t = c;
            if(i < a.size())
                t += a[i];
            c = (t < a[i] || data_[i] < t ? 1 : 0);
            data_[i] -= t;
        }
        shrink();
    }
    void subByData(const __Data & a){
        if(data_.empty()){
            data_ = a;
            return;
        }
        int c = 0;
        __Data r;
        for(size_t i = 0;i < data_.size() || c;++i){
            __Int t = c;
            if(i < data_.size())
                t += data_[i];
            c = (t < data_[i] || a[i] < t ? 1 : 0);
            r.push_back(a[i] - t);
        }
        r.swap(data_);
        shrink();
    }

    //fields
    __Data data_;
    bool sign_ = false;
};


#endif

