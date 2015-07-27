/*
 * Copyright (c) 2005 by DoZerg.  ALL RIGHTS RESERVED. 
 * Consult your license regarding permissions and restrictions.
 */
#pragma once

namespace DoZerg{
//declaration.
	template<long N>class SignedBiasNumber		//Bias Number with 1 bit for Sign.移码类，但最高位表示所在浮点数的符号，一般不参与运算。只适用于表示浮点数类的Sign和Exponent
	{
		static const long TOTAL_BYTES;
		static const long EACH_BITS;									
		static const unsigned long SignBit;							//Sign-Bit for Float Number
		static const unsigned long ExpSignBit;						//exponent's sign bit
		unsigned long data_[N];										//the highest byte is data_[0]
		unsigned long GetHighValue(bool) const;				//get data_[0] if set Sign-Bit to a bool value
		SignedBiasNumber & SetHighValue(unsigned long);		//set data_[0] to a value with Sign-Bit unchanged
		SignedBiasNumber & Minus();			//self -()
	//	SignedBiasNumber & Complement();		//self ~()
		const std::string ToString() const;
	public:
		~SignedBiasNumber(){}
		SignedBiasNumber(bool,long,bool = true);		//the 1st bool means Sign-Bit,the 2nd means whether it's normalized,true by default
		unsigned long SetSign(bool);					//set Sign-Bit to 1 if true,otherwise to 0
		bool Sign() const;						//return true if Sign-Bit is 0
		bool NonZero() const;
		bool ExpSign() const;					//return true if exponent>=0
		bool AllZero() const;
		bool AllOne() const;
		bool Denormalize() const;				//all 0 or all 1 in every bit except Sign-Bit
		const SignedBiasNumber operator +() const;
		const SignedBiasNumber operator -() const;
	//	const SignedBiasNumber operator ~() const;
		const SignedBiasNumber & operator ++();
		const SignedBiasNumber & operator --();
		const SignedBiasNumber & operator +=(const SignedBiasNumber &);
		const SignedBiasNumber & operator -=(const SignedBiasNumber &);
//temporary functions
		void show() const{
			for(long i=0;i<N;i++)
				cout<<hex<<data_[i]<<' ';
			cout<<endl;
		}
//friend functions.
		inline friend std::ostream & operator <<(std::ostream & os,const SignedBiasNumber & value){
			return os<<value.ToString();
		}
	};
	template<>class SignedBiasNumber<0>{};
//definition
//private:
	template<long N>
		const long SignedBiasNumber<N>::TOTAL_BYTES = N * sizeof(unsigned long);
	template<long N>
		const long SignedBiasNumber<N>::EACH_BITS = 8 * sizeof(unsigned long);
	template<long N>
		const unsigned long SignedBiasNumber<N>::SignBit = 1<<(EACH_BITS-1);
	template<long N>
		const unsigned long SignedBiasNumber<N>::ExpSignBit = SignBit>>1;
	template<long N>
		inline unsigned long SignedBiasNumber<N>::GetHighValue(bool value) const{
			return value?data_[0]|SignBit:data_[0]&~SignBit;
		}
	template<long N>
		inline SignedBiasNumber<N> & SignedBiasNumber<N>::SetHighValue(unsigned long value){
			data_[0]=SignBit&data_[0]|value;
			return *this;
		}
	template<long N>
		SignedBiasNumber<N> & SignedBiasNumber<N>::Minus(){
			unsigned long carry(1);
			for(long i=N-1;i>0;i--){
				data_[i]=~data_[i]-carry;
				carry&=data_[i]==-1;
			}
			return SetHighValue(~data_[0]-carry);
		}
/*	template<long N>
		SignedBiasNumber<N> & SignedBiasNumber<N>::Complement(){
			for(long i=1;i<N;++i)
				data_[i]=~data_[i];
			return SetHighValue(~data_[0]);
		}*/
	template<long N>
		const std::string SignedBiasNumber<N>::ToString() const{
			if(AllOne())
				return "INFINITE";
			if(!NonZero())
				return "0";
			unsigned long sign(data_[0]&ExpSignBit?1:0);
			std::string tmp;
			tmp.push_back(0);
			for(long j=3;j<=EACH_BITS;++j)
				StringHelper::MutiStringWithTwo(tmp,((data_[0]>>(-j))&1)==sign);
			for(long i=1;i<N-1;++i)
				for(long j=1;j<=EACH_BITS;++j)
					StringHelper::MutiStringWithTwo(tmp,((data_[i]>>(-j))&1)==sign);
			for(long j=1;j<EACH_BITS;++j)
				StringHelper::MutiStringWithTwo(tmp,((data_[N-1]>>(-j))&1)==sign);
			StringHelper::MutiStringWithTwo(tmp,sign+((data_[N-1]&1)==sign?1:0));
			StringHelper::ReverseString(tmp,48);
			return sign?tmp:"-"+tmp;
		}
//public:
	template<long N>
		SignedBiasNumber<N>::SignedBiasNumber(bool sign,long value,bool bias){	//if bias==false,value==0 means AllZero()==true,value!=0 means AllOne()==true
			if(bias){
				if(value){
					memset(data_,0,TOTAL_BYTES);
					data_[N-1]=abs(value)- 1;
				}else
					memset(data_,-1,TOTAL_BYTES);
				data_[0]^=ExpSignBit;
				if(value<0)
					Minus();
			}else
				memset(data_,value?-1:0,TOTAL_BYTES);
			SetSign(sign);
		}
	template<long N>
		inline unsigned long SignedBiasNumber<N>::SetSign(bool value){
			return value?data_[0]|=SignBit:data_[0]&=~SignBit;
		}
	template<long N>
		inline bool SignedBiasNumber<N>::Sign() const{
			return !(data_[0]&SignBit);
		}
	template<long N>
		bool SignedBiasNumber<N>::NonZero() const{
			if(AllZero())
				return false;
			if((GetHighValue(true)^ExpSignBit)!=-1)
				return true;
			for(long i=1;i<N;++i)
				if(data_[i]!=-1)
					return true;
			return false;
		}
	template<long N>
		inline bool SignedBiasNumber<N>::ExpSign() const{
			return data_[0]&ExpSignBit||!NonZero();
		}
	template<long N>
		bool SignedBiasNumber<N>::AllZero() const{
			for(long i=N-1;i>0;--i)
				if(data_[i])
					return false;
			return !GetHighValue(false);
		}
	template<long N>
		bool SignedBiasNumber<N>::AllOne() const{
			for(long i=N-1;i>0;--i)
				if(data_[i]!=-1)
					return false;
			return -1==GetHighValue(true);
		}
	template<long N>
		bool SignedBiasNumber<N>::Denormalize() const{
			if(N>1&&!data_[N-1]&&data_[N-1]!=-1)
				return false;
			for(long i=N-2;i>0;--i)
				if(data_[i]!=data_[N-1])
					return false;
			return	N>1?GetHighValue(data_[N-1]!=0)==data_[N-1]:!GetHighValue(false)||-1==GetHighValue(true);
		}
	template<long N>
		inline const SignedBiasNumber<N> SignedBiasNumber<N>::operator +() const{
			return SignedBiasNumber<N>(*this);
		}
	template<long N>
		inline const SignedBiasNumber<N> SignedBiasNumber<N>::operator -() const{
			return SignedBiasNumber<N>(*this).Minus();
		}
/*	template<long N>
		const SignedBiasNumber<N> SignedBiasNumber<N>::operator ~() const{
			return SignedBiasNumber<N>(*this).Complement();
		}*/
	template<long N>
		const SignedBiasNumber<N> & SignedBiasNumber<N>::operator ++(){
			long i=N-1;
			for(;i>0;i--)
				if(++data_[i])
					break;
			if(!i)
				SetHighValue(data_[0]+1);
			return *this;
		}
	template<long N>
		const SignedBiasNumber<N> & SignedBiasNumber<N>::operator --(){
			long i=N-1;
			for(;i>0;i--)
				if(data_[i]--)
					break;
			if(!i)
				SetHighValue(data_[0]-1);
			return *this;
		}
	template<long N>
		const SignedBiasNumber<N> & SignedBiasNumber<N>::operator +=(const SignedBiasNumber & value){
			unsigned long carry=1;				//for Bias Number,Bias(A+B)=Bias(A)+Bias(B)+S+1,where S is ExpSignBit
			for(long i=N-1;i>0;i--){
				unsigned long tmp=value.data_[i]+carry;
				data_[i]+=tmp;
				carry=tmp<carry||tmp>data_[i]?1:0;
			}
			return SetHighValue(data_[0]+value.data_[0]+carry).AddExpSign();
		}
	template<long N>
		const SignedBiasNumber<N> & SignedBiasNumber<N>::operator -=(const SignedBiasNumber & value){
			unsigned long carry(0);				//for Bias Number,Bias(A-B)=Bias(A)+~Bias(B)+S,where S is ExpSignBit
			for(long i=N-1;i>0;i--){
				unsigned long tmp=~value.data_[i]+carry;
				data_[i]+=tmp;
				carry=tmp<carry||tmp>data_[i]?1:0;
			}
			return SetHighValue(data_[0]+~value.data_[0]+carry).AddExpSign();
		}
//others:
	template<long N>
		inline const SignedBiasNumber<N> operator +(const SignedBiasNumber<N> & value1,const SignedBiasNumber<N> & value2){
			return SignedBiasNumber<N>(value1).operator +=(value2);
		}
	template<long N>
		inline const SignedBiasNumber<N> operator -(const SignedBiasNumber<N> & value1,const SignedBiasNumber<N> & value2){
			return SignedBiasNumber<N>(value1).operator -=(value2);
		}

}//namespace DoZerg