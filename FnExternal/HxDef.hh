#pragma once
typedef char int8;
typedef unsigned char uint8;
typedef short  int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef __int64 int64;
typedef unsigned __int64 uint64;

#define _BYTE  uint8
#define _WORD  uint16
#define _DWORD uint32
#define _QWORD uint64
#define FortPTR uintptr_t
#define FortStorage ptrdiff_t
#define NULL 0
#define LAST_IND(x,part_type)    (sizeof(x)/sizeof(part_type) - 1)
#define HIGH_IND(x,part_type)  LAST_IND(x,part_type)
#define LOW_IND(x,part_type)   0
#define BYTEn(x, n)   (*((_BYTE*)&(x)+n))
#define WORDn(x, n)   (*((_WORD*)&(x)+n))
#define DWORDn(x, n)  (*((_DWORD*)&(x)+n))
#define LODWORD(x) DWORDn(x,LOW_IND(x,_DWORD))
#define HIDWORD(x) DWORDn(x,HIGH_IND(x,_DWORD))
#define BYTE1(x)   BYTEn(x,  1)        
#define BYTE2(x)   BYTEn(x,  2)
#define BYTE3(x)   BYTEn(x,  3)
#define BYTE4(x)   BYTEn(x,  4)
#define BYTE5(x)   BYTEn(x,  5)
#define BYTE6(x)   BYTEn(x,  6)
#define BYTE7(x)   BYTEn(x,  7)
#define BYTE8(x)   BYTEn(x,  8)
#define BYTE9(x)   BYTEn(x,  9)
#define BYTE10(x)  BYTEn(x, 10)
#define BYTE11(x)  BYTEn(x, 11)
#define BYTE12(x)  BYTEn(x, 12)
#define BYTE13(x)  BYTEn(x, 13)
#define BYTE14(x)  BYTEn(x, 14)
#define BYTE15(x)  BYTEn(x, 15)
#define WORD1(x)   WORDn(x,  1)
#define WORD2(x)   WORDn(x,  2)         
#define WORD3(x)   WORDn(x,  3)
#define WORD4(x)   WORDn(x,  4)
#define WORD5(x)   WORDn(x,  5)
#define WORD6(x)   WORDn(x,  6)
#define WORD7(x)   WORDn(x,  7)

template<class T>  int16 __PAIR__(int8  high, T low) { return (((int16)high) << sizeof(high) * 8) | uint8(low); }
template<class T>  int32 __PAIR__(int16 high, T low) { return (((int32)high) << sizeof(high) * 8) | uint16(low); }
template<class T>  int64 __PAIR__(int32 high, T low) { return (((int64)high) << sizeof(high) * 8) | uint32(low); }
template<class T> uint16 __PAIR__(uint8  high, T low) { return (((uint16)high) << sizeof(high) * 8) | uint8(low); }
template<class T> uint32 __PAIR__(uint16 high, T low) { return (((uint32)high) << sizeof(high) * 8) | uint16(low); }
template<class T> uint64 __PAIR__(uint32 high, T low) { return (((uint64)high) << sizeof(high) * 8) | uint32(low); }

template<class T> T __ROL__(T value, int count)
{
	const uint32 nbits = sizeof(T) * 8;

	if (count > 0)
	{
		count %= nbits;
		T high = value >> (nbits - count);
		if (T(-1) < 0) // signed value
			high &= ~((T(-1) << count));
		value <<= count;
		value |= high;
	}
	else
	{
		count = -count % nbits;
		T low = value << (nbits - count);
		value >>= count;
		value |= low;
	}
	return value;
}

__forceinline uint8  __ROL1__(uint8  value, int count) { return __ROL__((uint8)value, count); }
__forceinline uint16 __ROL2__(uint16 value, int count) { return __ROL__((uint16)value, count); }
__forceinline uint32 __ROL4__(uint32 value, int count) { return __ROL__((uint32)value, count); }
__forceinline uint64 __ROL8__(uint64 value, int count) { return __ROL__((uint64)value, count); }
__forceinline uint8  __ROR1__(uint8  value, int count) { return __ROL__((uint8)value, -count); }
__forceinline uint16 __ROR2__(uint16 value, int count) { return __ROL__((uint16)value, -count); }
__forceinline uint32 __ROR4__(uint32 value, int count) { return __ROL__((uint32)value, -count); }
__forceinline uint64 __ROR8__(uint64 value, int count) { return __ROL__((uint64)value, -count); }


char* wchar_to_char(const wchar_t* pwchar)
{
	int currentCharIndex = 0;
	char currentChar = pwchar[currentCharIndex];

	while (currentChar != '\0')
	{
		currentCharIndex++;
		currentChar = pwchar[currentCharIndex];
	}

	const int charCount = currentCharIndex + 1;

	char* filePathC = (char*)malloc(sizeof(char) * charCount);

	for (int i = 0; i < charCount; i++)
	{
		char character = pwchar[i];

		*filePathC = character;

		filePathC += sizeof(char);

	}
	filePathC += '\0';

	filePathC -= (sizeof(char) * charCount);

	return filePathC;
}
