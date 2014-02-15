/*
 * Copyright (c) 2005 by DoZerg.  ALL RIGHTS RESERVED. 
 * Consult your license regarding permissions and restrictions.
 */
#ifndef DOZERG_SIGNED_HUGE_LONG_H_20080924
#define DOZERG_SIGNED_HUGE_LONG_H_20080924

#include "HugeNumberBase.h"

namespace DoZerg{
	template<
		long N,
		typename DataType = DataTraits::__DefaultDataType
	>class SignedHugeLong:public HugeNumberBase<N,DataType>
	{
        typedef HugeNumberBase<N,DataType>      __MyBase;
        typedef SignedHugeLong<N,DataType>      __Myt;
        typedef typename __MyBase::__Signed     __Signed;
        typedef typename __MyBase::__Unsigned   __Unsigned;
    public:
        SignedHugeLong(){}
        SignedHugeLong(const __Signed & value):__MyBase(value,true){}
        SignedHugeLong(const std::string & value):__MyBase(value){}
        SignedHugeLong(const __Myt & value):__MyBase(value){}
        explicit SignedHugeLong(const __MyBase & value):__MyBase(value){}
        __Myt & operator =(const __Signed & value){
            __MyBase::FromValue(value,false);
            return *this;
        }
        __Myt & operator =(const std::string & value){
            __MyBase::FromString(value);
            return *this;
        }
        __Myt & operator =(const __Myt & value){
            if(this != &value)
                __MyBase::FromSelf(value);
            return *this;
        }
        __Myt & operator =(const __MyBase & value){
            if(this != &value)
                __MyBase::FromSelf(value);
            return *this;
        }
        __Myt operator +() const{return __Myt(*this);}
        __Myt operator -() const{return __Myt(*this).minus();}
        __Myt operator ~() const{return __Myt(*this).complement();}
        __Myt operator >>(int value) const{return __Myt(*this).rightShift(value);}
        __Myt operator <<(int value) const{return __Myt(*this).leftShift(value);}
        __Myt & operator ++(){__MyBase::Increase();return *this;}
        __Myt & operator --(){__MyBase::Decrease();return *this;}
        __Myt operator ++(int){__Myt tmp(*this);operator ++();return tmp;}
        __Myt operator --(int){__Myt tmp(*this);operator --();return tmp;}
        __Myt & operator >>=(int value){return rightShift(value,false);}
        __Myt & operator <<=(int value){return leftShift(value);}
        __Myt & operator +=(const __Myt & value){__MyBase::OperatorAdd(value);return *this;}
        __Myt & operator -=(const __Myt & value){__MyBase::OperatorSub(value);return *this;}
        __Myt & operator *=(const __Myt & value){__MyBase::OperatorMult(value);return *this;}
        __Myt & operator /=(const __Myt & value){
            const bool sign(__MyBase::GetSignBit());
            if(sign)
                __MyBase::Minus();
            OperatorDiv(abs(value));
            if(sign ^ value.GetSignBit())
                __MyBase::Minus();
            return *this;
        }
        __Myt & operator %=(const __Myt & value){
            const bool sign(__MyBase::GetSignBit());
            if(sign)
                __MyBase::Minus();
            OperatorDiv(abs(value),false);
            if(sign)
                __MyBase::Minus();
            return *this;
        }
        __Myt & operator &=(const __Myt & value){__MyBase::OperatorAnd(value);return *this;}
        __Myt & operator |=(const __Myt & value){__MyBase::OperatorOr(value);return *this;}
        __Myt & operator ^=(const __Myt & value){__MyBase::OperatorXor(value);return *this;}
        std::string ToString(int BaseValue = 10,bool uppercase = false,bool showbase = false) const{
            return __MyBase::ToString(BaseValue,true,uppercase,showbase);
        }
        bool operator ==(const __Myt & value) const{return __MyBase::OperatorEqual(value);}
        bool operator !=(const __Myt & value) const{return !operator ==(value);}
        bool operator <(const __Myt & value) const{return __MyBase::OperatorSmaller(value);}
        bool operator >(const __Myt & value) const{return value.operator <(*this);}
        bool operator <=(const __Myt & value) const{return !value.operator <(*this);}
        bool operator >=(const __Myt & value) const{return !operator <(value);}
        __Myt operator +(const __Myt & value) const{return __Myt(*this).operator +=(value);}
        __Myt operator -(const __Myt & value) const{return __Myt(*this).operator -=(value);}
        __Myt operator *(const __Myt & value) const{return __Myt(*this).operator *=(value);}
        __Myt operator /(const __Myt & value) const{return __Myt(*this).operator /=(value);}
        __Myt operator %(const __Myt & value) const{return __Myt(*this).operator %=(value);}
        __Myt operator &(const __Myt & value) const{return __Myt(*this).operator &=(value);}
        __Myt operator |(const __Myt & value) const{return __Myt(*this).operator |=(value);}
        __Myt operator ^(const __Myt & value) const{return __Myt(*this).operator ^=(value);}
        //friend functions
        inline friend bool operator ==(const __Signed & a,const __Myt & b){return __Myt(a) == b;}
        inline friend bool operator !=(const __Signed & a,const __Myt & b){return __Myt(a) != b;}
        inline friend bool operator <(const __Signed & a,const __Myt & b){return __Myt(a) < b;}
        inline friend bool operator >(const __Signed & a,const __Myt & b){return __Myt(a) > b;}
        inline friend bool operator <=(const __Signed & a,const __Myt & b){return __Myt(a) <= b;}
        inline friend bool operator >=(const __Signed & a,const __Myt & b){return __Myt(a) >= b;}
        inline friend __Myt operator +(const __Signed & a,const __Myt & b){return __Myt(a) += b;}
        inline friend __Myt operator -(const __Signed & a,const __Myt & b){return __Myt(a) -= b;}
        inline friend __Myt operator *(const __Signed & a,const __Myt & b){return __Myt(a) *= b;}
        inline friend __Myt operator /(const __Signed & a,const __Myt & b){return __Myt(a) /= b;}
        inline friend __Myt operator %(const __Signed & a,const __Myt & b){return __Myt(a) %= b;}
        inline friend __Myt operator &(const __Signed & a,const __Myt & b){return __Myt(a) &= b;}
        inline friend __Myt operator |(const __Signed & a,const __Myt & b){return __Myt(a) |= b;}
        inline friend __Myt operator ^(const __Signed & a,const __Myt & b){return __Myt(a) ^= b;}
        inline friend __Myt abs(const __Myt & a){return (a.GetSignBit() ? -value : value);}
        inline friend std::ostream & operator <<(std::ostream & os,const __Myt & value){
            const std::ios::fmtflags fmt = os.flags();
            const int base = (fmt & os.hex ? 16 : (fmt & os.oct ? 8 : 10));
            return os<<value.ToString(base,(fmt & os.uppercase) != 0,(fmt & os.showbase) != 0);
        }
    private:
        __Myt & minus(){__MyBase::Minus();return *this;}
        __Myt & complement(){__MyBase::Complement();return *this;}
        __Myt & rightShift(int value){__MyBase::RightShift(value,false);return *this;}
        __Myt & leftShift(int value){__MyBase::LeftShift(value);return *this;}
	};
}	//namespace DoZerg

#endif
