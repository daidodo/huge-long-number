/*
 * Copyright (c) 2005 by DoZerg.  ALL RIGHTS RESERVED. 
 * Consult your license regarding permissions and restrictions.
 */
#pragma once
#include "UnsignedHugeLong.h"
#include "SignedBiasNumber.h"

namespace DoZerg{
/*/declaration
	template<long E,long N>class HugeFloat{
		SignedBiasNumber<E> exp_;			//the first bit is Sign-Bit,else are exponent,using Biased Representation
		UnsignedHugeLong<N> data_;			//fraction
		static const unsigned long SignBit;
		static const long ExpBytes;			//bytes of exp_
		static const long DataBytes;		//bytes of data_
	public:
		HugeFloat(){};
		~HugeFloat(){}
		HugeFloat(double);
		HugeFloat(const std::string &);
		const HugeFloat & operator =(const std::string &);

//temporary functions
		void show() const{
			cout<<"Exp:	"<<exp_<<endl
				<<"Data:	"<<data_<<endl;
		}

	};
//definition
//private:
	template<long N,long E>
		const unsigned long HugeFloat<N,E>::SignBit = 1<<((sizeof(unsigned long)<<3)-1);
	template<long N,long E>
		const long HugeFloat<N,E>::ExpBytes = E * sizeof(unsigned long);
	template<long N,long E>
		const long HugeFloat<N,E>::DataBytes = N * sizeof(unsigned long);
//public:
	template<long N,long E>
		HugeFloat<N,E>::HugeFloat(double value):exp_(value<0,DoubleHelper::DoubleExp(value)){
		}
	template<long N,long E>
		HugeFloat<N,E>::HugeFloat(const std::string & value){
		}


	typedef HugeFloat<1,2> hfloat3;
	typedef HugeFloat<1,4> hfloat5;
	typedef HugeFloat<1,8> hfloat9;
	typedef HugeFloat<1,16> hfloat17;	//*/
}//namespace DoZerg
