/*
* Copyright (c) 2005 by DoZerg.  ALL RIGHTS RESERVED. 
* Consult your license regarding permissions and restrictions.
*/
#ifndef DOZERG_HELPER_H_20080925
#define DOZERG_HELPER_H_20080925

#include <string>

namespace DoZerg{
    namespace StringHelper{
        int CheckBaseFromString(const std::string & value){
            int base = 0;
            for(std::string::const_iterator v = value.begin();v != value.end() && base != -1; ++v)
                switch(base){
                    case 0:
                        base = ('+'==*v || '-'==*v)?1:('0'==*v)?2:(*v>'0' && *v<='9')?10:-1;
                        break;
                    case 1:
                        base = ('0'==*v)?2:(*v>'0' && *v<='9')?10:-1;   //only '+' or '-'
                        break;
                    case 2:
                        base = ('x'==*v || 'X'==*v)?3:(*v>='0' && *v<'8')?8:-1; //just 0
                        break;
                    case 3:
                        base = ((*v>'0' && *v<='9')||(*v>='a' && *v<='f')||(*v>='A' && *v<='F'))?16:-1;    //0x
                        break;
                    case 8:
                        base = (*v>='0' && *v<'8')?8:-1;
                        break;
                    case 10:
                        base = (*v>='0' && *v<='9')?10:-1;
                        break;
                    case 16:
                        base = ((*v>='0' && *v<='9')||(*v>='a' && *v<='f')||(*v>='A' && *v<='F'))?16:-1;
                        break;
                    default:;
            }
            return base;
        }
        int DivideStringByTwo(std::string & value,bool & IsZero){  //return the highest bit,then divide the string  by 2,in reverse order
            bool carry(false);
            for(std::string::reverse_iterator v = value.rbegin();v != value.rend();++v){
                if(carry)
                    *v += 10;
                carry = *v & 1;
                *v >>= 1;
            }
            std::string::size_type p(value.find_last_not_of(char(0)) + 1);
            IsZero =! p;
            value.erase(p);
            return (carry ? 1 : 0);
        }
        void MutiStringWithTwo(std::string & value,int AddNumber){  //multiple the string,then add the number(0 to 9),in reverse order
            int carry(AddNumber);
            for(std::string::iterator v = value.begin();v != value.end();++v){
                *v = (*v << 1) + carry;
                if(carry = *v / 10)
                    *v %= 10;
            }
            if(carry)
                value.push_back(carry); //this requires 0<=carry<=9
        }
        std::string & ReverseString(std::string & value,int AddValue = 0){  //reverse the string,and add AddValue to all elements
            std::string::size_type l=value.length();
            if(l & 1)
                value[l / 2] += AddValue;
            for(std::string::size_type i = 0;i < l / 2;++i){        
                value[i] ^= value[l - i - 1];
                value[l - i - 1] ^= value[i];
                value[i] ^= value[l - i - 1];
                value[i] += AddValue;
                value[l - i - 1] += AddValue;
            }
            return value;
        }
    }//namespace StringHelper
#define SPECIFIED_DataTypeTrait_FOR_S(type)	\
	template<>struct DataTypeTrait<type>{	\
		typedef type			__Signed;	\
		typedef unsigned type	__Unsigned;	\
	};	\
	template<>struct DataTypeTrait<unsigned type>{	\
		typedef type			__Signed;	\
		typedef unsigned type	__Unsigned;	\
	};
	namespace DataTraits{
		typedef long long __DefaultDataType;
		//struct DataType
        template<typename Integer>struct DataTypeTrait{};
		SPECIFIED_DataTypeTrait_FOR_S(long long);
		SPECIFIED_DataTypeTrait_FOR_S(long);
		SPECIFIED_DataTypeTrait_FOR_S(int);
		SPECIFIED_DataTypeTrait_FOR_S(short);
		SPECIFIED_DataTypeTrait_FOR_S(char);
		template<>struct DataTypeTrait<signed char>{
			typedef signed char		__Signed;
			typedef unsigned char	__Unsigned;
		};
	}//namespace DataTraits
#undef SPECIFIED_DataTypeTrait_FOR_S
}//namespace DoZerg

#endif
