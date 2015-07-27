/*
* Copyright (c) 2005 by DoZerg.  ALL RIGHTS RESERVED. 
* Consult your license regarding permissions and restrictions.
*/
#ifndef DOZERG_HUGE_NUMBER_BASE_H_20080924
#define DOZERG_HUGE_NUMBER_BASE_H_20080924

#include <stdexcept>    //for std::overflow_error,std::runtime_error
#include "Helper.h"

namespace DoZerg{
    template<int N,typename T>
    class HugeNumberBase
    {
        typedef HugeNumberBase<N,T>             __Myt;
        typedef DataTraits::DataTypeTrait<T>    __DTT;
        typedef void (__Myt::*__safe_bool_type)(int);
    protected:
        typedef typename __DTT::__Signed        __Signed;
        typedef typename __DTT::__Unsigned      __Unsigned;
    private:
        static const size_t EACH_BYTES = sizeof(__Signed);  //bytes per data_
        static const size_t EACH_BITS = 8 * EACH_BYTES;     //bits per data_
        static const size_t TOTAL_BITS = N * EACH_BITS;     //total bits of data_[N]
        static const __Unsigned SignBit = __Unsigned(1) << (EACH_BITS - 1);
        __Unsigned data_[N];    //the highest data is data_[0],and the lowest is data_[N-1]
    public:
        static const size_t TOTAL_BYTES = N * EACH_BYTES;   //total bytes of data_[N]
        __Signed ToSigned() const               {return data_[N - 1];}
        const __Unsigned & ToUnsigned() const   {return data_[N - 1];}
        bool GetSignBit() const                 {return (data_[0] & SignBit) != 0;}
        operator __safe_bool_type() const       {return operator !() ? 0 : &__Myt::addOne;}
        bool operator !() const{
            for(int i = 0;i < N;++i)
                if(data_[i] != 0)
                    return false;
            return true;

        }
        //friend functions
        friend std::istream & operator >>(std::istream & is,__Myt & value){
            std::string tmp;
            is>>tmp;
            value.FromString(tmp);
            return is;
        }
    protected:
        HugeNumberBase(){}
        HugeNumberBase(const __Myt & value){FromSelf(value);}
        HugeNumberBase(const __Signed & value,bool sign){FromValue(value,sign);}
        explicit HugeNumberBase(const std::string & value){FromString(value);}
        void FromSelf(const __Myt & value){
            memcpy(data_,value.data_,TOTAL_BYTES);
        }
        void FromValue(__Signed value,bool sign){
            reset();
            if(sign){
                if(value < 0)
                    value = -value;
                else
                    sign = false;
            }
            data_[N - 1] = __Unsigned(value);
            if(sign)
                Minus();
        }
        void FromString(const std::string & value){
            const int ASCII_0 = '0';
            switch(StringHelper::CheckBaseFromString(value)){    //this function ensures the data format
                case 2:{        //0
                    reset();
                    break;}
                case 8:{
                    reset();
                    int at(N - 1),bits(0);
                    for(std::string::const_reverse_iterator v = value.rbegin();at >= 0 && *v != '+' && *v != '-' && v != value.rend();++v){
                        __Unsigned t((*v - ASCII_0) & 7);
                        data_[at] += t << bits;
                        if((bits += 3) >= EACH_BITS){
                            --at;
                            if(bits -= EACH_BITS)
                                data_[at] += (t >> (3 - bits));
                        }    //if at < 0 then the passed value is too large    
                    }break;}
                case 10:{
                    reset();
                    std::string tmp(value);
                    if(tmp[0] == '+' || tmp[0] == '-')
                        tmp[0] = ASCII_0;
                    StringHelper::ReverseString(tmp,-ASCII_0);
                    bool zero;
                    int at(N - 1),bits(0);
                    do{
                        data_[at] += __Unsigned(StringHelper::DivideStringByTwo(tmp,zero)) << bits;
                        if(++bits >= EACH_BITS){
                            --at;
                            bits = 0;
                        }    
                    }while(!zero && at >= 0);   //if !zero then the passed value is too large
                    break;}
                case 16:{
                    reset();
                    int at(N - 1),bits(0);
                    for(std::string::const_reverse_iterator v = value.rbegin();*v != 'x' && *v != 'X' && at >= 0;++v){
                        __Unsigned t(((*v >= '0' && *v <= '9') ? *v - '0' : (*v >= 'a' && *v <= 'f') ? *v - 'a' + 10 : *v - 'A' + 10) & 0xF);
                        data_[at] += (t << bits);
                        if((bits += 4) >= EACH_BITS){
                            --at;
                            if(bits -= EACH_BITS)
                                data_[at] += (t >> (4 - bits));
                        }    
                    }break;}
                default:{
                    throw std::runtime_error("The passed data format is unknown"); 
                }        
            }
            if(value[0] == '-')
                Minus();
        }
        void Increase(){    //operator ++()
            for(int i = N - 1;i >= 0 && !++data_[i];--i);
        }
        void Decrease(){    //operator --()
            int i = N - 1;
            for(;i >= 0 && !data_[i];--i);
            if(i < 0)
                reset(-1);
            else
                --data_[i];
        }
        __Myt & Complement(){   //self ~()
            for(int i = 0;i < N;++i)
                data_[i] = ~data_[i];
            return *this;
        }
        __Myt & Minus(){    //self -()
            __Unsigned carry(1);
            for(int i = N - 1;i >= 0;--i){
                data_[i] = ~data_[i] + carry;
                carry = (data_[i] < carry ? __Unsigned(1) : __Unsigned(0));
            }
            return *this;
        }
        __Myt & RightShift(int value,bool bsigned){ //the bool means whether to treat *this as a signed value    //O(N)
            int rshift = value % TOTAL_BITS;
            if(!rshift)
                return *this;
            if(rshift < 0)
                rshift += TOTAL_BITS;
            int at = N - 1,start = rshift / EACH_BITS;
            rshift %= EACH_BITS;
            for(int i = N - start - 1;i > 0;--i)
                data_[at--] = (rshift != 0 ? (data_[i] >> rshift) + (data_[i - 1] << (EACH_BITS - rshift)) : data_[i]);
            if(bsigned){
                data_[at--] = __Signed(data_[0]) >> rshift;
                __Signed t(GetSignBit() ? -1 : 0);
                for(int i = at;i >= 0;--i)
                    data_[i] = t;
            }else{
                data_[at--] = data_[0] >> rshift;
                for(int i = at;i >= 0;--i)
                    data_[i] = 0;
            }
            return *this;
        }
        __Myt & LeftShift(int value){
            int lshift = value % TOTAL_BITS;
            if(!lshift)
                return *this;
            if(lshift < 0)
                lshift += TOTAL_BITS;
            int at = 0,start = lshift / EACH_BITS;
            lshift %= EACH_BITS;
            for(int i = start;i < N - 1;++i)
                data_[at++] = lshift ? (data_[i] << lshift) + (data_[i + 1] >> (EACH_BITS - lshift)):data_[i];
            data_[at++] = data_[N - 1] << lshift;
            for(int i = at;i < N;++i)
                data_[i] = 0;
            return *this;
        }
        __Myt & OperatorAdd(const __Myt & value){
            __Unsigned carry(0);
            for(int i = N - 1;i >= 0;--i){
                __Unsigned tmp(value.data_[i] + carry);
                data_[i] += tmp;
                carry = (tmp < carry || tmp > data_[i] ? 1 : 0);
            }
            return *this;        //if carry then overflow
        }
        __Myt & OperatorSub(const __Myt & value){
            __Unsigned carry(0);
            for(int i = N - 1;i >= 0;--i){
                __Unsigned tmp(value.data_[i] + carry);
                carry = (tmp > data_[i] || tmp < value.data_[i] ? 1 : 0);
                data_[i] -= tmp;
            }
            return *this;        //if carry then overflow
        }
        __Myt & OperatorMult(const __Myt & value){
            __Myt tmp(*this),tmpvalue(value);        //be aware of self mult like x.OperatorMult(x)
            reset();
            __Signed carry(0);
            for(int i = N - 1,at = 0;i >= 0;--i)
                for(int j = 0;j < EACH_BITS;j += 2,at += 2){
                    __Signed t(((tmpvalue.data_[i] >> j) & 3) + carry - 1);        //-------------------------------
                    if(!t || t == 1){
                        OperatorAdd(tmp);
                        if(t == 1)
                            OperatorAdd(tmp);
                    }else if(t == 2)
                        OperatorSub(tmp);
                    tmp.LeftShift(2);
                    carry = (t > 1 ? 1 : 0);
                }
                return *this;
        }
        __Myt & OperatorAnd(const __Myt & value){
            for(int i = 0;i < N;++i)
                data_[i] &= value.data_[i];
            return *this;
        }
        __Myt & OperatorOr(const __Myt & value){
            for(int i = 0;i < N;++i)
                data_[i] |= value.data_[i];
            return *this;
        }
        __Myt & OperatorXor(const __Myt & value){
            for(int i = 0;i < N;++i)
                data_[i] ^= value.data_[i];
            return *this;
        }
        __Myt & OperatorDiv(const __Myt & value,bool result = true){    //return divided result if bool==true by default,otherwise return the modulus
            if(!value)
                throw std::overflow_error("Divided by Zero");
            int xshift = value.highestBit();
            if(xshift == value.lowestBit())
                return (result ? RightShift(xshift) : resetHigherBits(xshift));
            xshift = highestBit() - xshift;
            __Myt tmp(0);
            if(xshift >= 0){
                __Myt tmpvalue(value);
                tmpvalue.LeftShift(xshift);
                bool bsign = GetSignBit() ^ tmpvalue.GetSignBit();
                OperatorSub(tmpvalue);
                for(bsign ^= GetSignBit();xshift > 0 && NonZero();--xshift,bsign = GetSignBit()){
                    tmpvalue.rightShift1();
                    if(bsign)
                        OperatorAdd(tmpvalue);
                    else{
                        OperatorSub(tmpvalue);
                        tmp.addOne(xshift);
                    }    
                }
                if(bsign)
                    OperatorAdd(tmpvalue);
                else
                    tmp.addOne(xshift);
            }
            if(result)
                operator =(tmp);
            return *this;
        }
        __Myt & OperatorPower(const __Myt & value){
            if(!value)
                return FromInteger(1);
            __Myt tmp(*this),tmpvalue(value);        //be aware of self power like x.OperatorPower(x)
            for(int at = tmpvalue.highestBit() - 1;at >= 0;--at){
                OperatorMult(*this);
                if(((tmpvalue.data_[N - 1 - at / EACH_BITS] >> at) & 1) != 0)
                    OperatorMult(tmp);
            }
            return *this;
        }
        bool OperatorEqual(const __Myt & value) const{
            return !memcmp(data_,value.data_,TOTAL_BYTES);
        }
        bool OperatorSmaller(const __Myt & value,bool sign = false) const{
            if(sign){
                const bool mys = GetSignBit();
                const bool notsame = mys ^ value.GetSignBit();
                if(notsame)
                    return mys;
            }
            for(int i = 0;i < N;++i)
                if(data_[i] != value.data_[i])
                    return data_[i] < value.data_[i];
            return false;
        }
        std::string ToString(int BaseValue,bool bsigned,bool uppercase,bool showbase) const{            //the bool means whether to treat *this as a signed value
            const int ASCII_0 = '0';
            std::string tmp,prefix;
            switch(BaseValue){
                case 8:{
                    if(showbase)
                        prefix.push_back(ASCII_0);
                    for(int i = N - 1,shift = 0;i >= 0;){
                        __Unsigned t((data_[i] >> shift) & 7);
                        tmp.push_back(int(t) + ASCII_0);        //---------------------------------------
                        shift += 3;
                        if(shift + 3 >= EACH_BITS){
                            __Unsigned tt(data_[i] >> shift);
                            shift += 3 - EACH_BITS;
                            if(--i >= 0)
                                tt += (((__Unsigned(1) << shift) - 1) & data_[i]) << (3 - shift);
                            tmp.push_back(int(tt) + ASCII_0);
                        }
                    }
                    std::string::size_type pos(tmp.find_last_not_of(ASCII_0) + 1);    //pos==0 if all '0'
                    tmp.erase(pos ? pos : 1);
                    StringHelper::ReverseString(tmp);
                    break;}
                case 16:{
                    if(showbase)
                        prefix = uppercase ? "0X" : "0x";
                    bool nonzero(false);
                    for(int i = 0;i < N;++i){
                        if(!data_[i]){
                            if(nonzero){
                                char t[EACH_BYTES * 2 + 1] = {0};
                                memset(t,ASCII_0,EACH_BYTES * 2);
                                tmp += t;
                            }
                            continue;
                        }
                        for(int j = EACH_BITS - 4;j >= 0 ;j -= 4){
                            __Unsigned t((data_[i] >> j) & 0xF);
                            if(!t){
                                if(nonzero)
                                    tmp.push_back(ASCII_0);
                                continue;
                            }
                            nonzero = true;
                            tmp.push_back((uppercase ? "0123456789ABCDEF" : "0123456789abcdef")[size_t(t)]);
                        }    
                    }
                    if(!nonzero)        //just 0
                        tmp.push_back(ASCII_0);
                    break;}
                default:{
                    tmp.push_back(0);
                    if(bsigned && GetSignBit()){
                        for(int i = 0;i < N - 1;++i)
                            for(int j = EACH_BITS - 1;j >= 0;--j)
                                StringHelper::MutiStringWithTwo(tmp,(((data_[i] >> j) & 1) != 0 ? 0: 1));    //----------------
                            for(int j = EACH_BITS - 1;j > 0;--j)
                                StringHelper::MutiStringWithTwo(tmp,(((data_[N - 1] >> j) & 1) != 0 ? 0: 1));        //---------------
                            StringHelper::MutiStringWithTwo(tmp,(data_[N-1] & 1) != 0 ? 1 : 2);
                            tmp.push_back(int('-') - ASCII_0);        //add '-'(45-48=-3)
                            StringHelper::ReverseString(tmp,ASCII_0);
                    }else{
                        for(int i = 0;i < N;++i)
                            for(int j = EACH_BITS - 1;j >= 0;--j){
                                StringHelper::MutiStringWithTwo(tmp,(((data_[i] >> j) & 1) != 0 ? 1: 0));    //-------------------
                            }
                            StringHelper::ReverseString(tmp,ASCII_0);
                    }    
                }    
            }
            return prefix + tmp;
        }
    private:
        void reset(int v = 0){
            memset(data_,v,TOTAL_BYTES);
        }
        void addOne(int Position){  //add 1 in a particular position
            int at(N - 1 - Position / EACH_BITS);
            __Unsigned tmp(data_[at]);
            if((data_[at--] += (__Unsigned(1) << (Position % EACH_BITS))) < tmp)
                while(at >= 0 && !++data_[at])
                    --at;      //if at<0 then overflow
        }
        int highestBit() const{ //return the highest bit position based on 0
            int at=0;
            while(at < N && !data_[at])
                ++at;
            if(at >= N)
                return 0;
            int t(0);
            while(!((data_[at] << t) & SignBit))
                ++t;
            return (N - at) * EACH_BITS - t - 1;
        }
        int lowestBit() const{  //return the lowest bit position based on 0
            int at(N - 1);
            while(at >= 0 && !data_[at])
                --at;
            if(at < 0)
                return 0;
            int t(0);
            while(!((data_[at] >> t) & 1))
                ++t;
            return (N - 1 - at) * EACH_BITS + t;
        }
        __Myt & resetHigherBits(int Position){  //reset the higher bits than a value to 0
            int at(N - 1 - Position / EACH_BITS);
            for(int i = 0;i < at;++i)
                data_[i] = 0;
            data_[at] &= (__Unsigned(1) << (Position % EACH_BITS)) - 1;
            return *this;
        }
        __Myt & rightShift1(){   //right shift 1 bit as a unsigned value(see data_'s declaration)
            for(int i = N - 1;i > 0;--i)
                data_[i] = ((data_[i] >> 1) + ((data_[i - 1] & 1) != 0 ? SignBit : 0));
            data_[0] >>= 1;
            return *this;
        }
    };
}    //namespace DoZerg

#endif
