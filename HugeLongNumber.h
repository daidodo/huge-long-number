/*
 * Copyright (c) 2005 by DoZerg.  ALL RIGHTS RESERVED. 
 * Consult your license regarding permissions and restrictions.
 */

#ifndef DOZERG_HUGE_LONG_NUMBER_H_20080924
#define DOZERG_HUGE_LONG_NUMBER_H_20080924

#include "UnsignedHugeLong.h"
#include "SignedHugeLong.h"

namespace DoZerg{
//typedefs:
	typedef UnsignedHugeLong<1>		ulong0;
	typedef UnsignedHugeLong<2>		ulong1;
	typedef UnsignedHugeLong<4>		ulong2;
	typedef UnsignedHugeLong<8>		ulong3;
	typedef UnsignedHugeLong<16>	ulong4;
	typedef UnsignedHugeLong<32>	ulong5;
	typedef UnsignedHugeLong<64>	ulong6;
	typedef UnsignedHugeLong<128>	ulong7;
	typedef UnsignedHugeLong<256>	ulong8;
	typedef UnsignedHugeLong<512>	ulong9;
	typedef UnsignedHugeLong<1024>	ulong10;
//typedefs:
	typedef SignedHugeLong<1>		slong0;
	typedef SignedHugeLong<2>		slong1;
	typedef SignedHugeLong<4>		slong2;
	typedef SignedHugeLong<8>		slong3;
	typedef SignedHugeLong<16>		slong4;
	typedef SignedHugeLong<32>		slong5;
	typedef SignedHugeLong<64>		slong6;
	typedef SignedHugeLong<128>		slong7;
	typedef SignedHugeLong<256>		slong8;
	typedef SignedHugeLong<512>		slong9;
	typedef SignedHugeLong<1024>	slong10;
}//namespace DoZerg

#endif
