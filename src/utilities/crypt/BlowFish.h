// blowfish.h     interface file for blowfish.cpp
// _THE BLOWFISH ENCRYPTION ALGORITHM_
// by Bruce Schneier
// Revised code--3/20/94
// Converted to C++ class 5/96, Jim Conger

#ifndef BLOWFISH_H_INCLUDED_
#define BLOWFISH_H_INCLUDED_

#define MAXKEYBYTES 	56		// 448 bits max
#define NPASS           16		// SBox passes

#define DWORD  		unsigned long
#define WORD  		unsigned short
#define BYTE  		unsigned char

class CBlowFish
{
// choose a byte order for your hardware
#define ORDER_DCBA	// chosing Intel in this case

#ifdef ORDER_DCBA  	// DCBA - little endian - intel

	union aword {
	  DWORD dword;
	  BYTE byte [4];
	  struct {
	    unsigned int byte3:8;
	    unsigned int byte2:8;
	    unsigned int byte1:8;
	    unsigned int byte0:8;
	  } w;
	};

#elif ORDER_ABCD  	// ABCD - big endian - motorola

	union aword {
	  DWORD dword;
	  BYTE byte [4];
	  struct {
	    unsigned int byte0:8;
	    unsigned int byte1:8;
	    unsigned int byte2:8;
	    unsigned int byte3:8;
	  } w;
	};

#elif ORDER_BADC  	// BADC - vax

	union aword {
	  DWORD dword;
	  BYTE byte [4];
	  struct {
	    unsigned int byte1:8;
	    unsigned int byte0:8;
	    unsigned int byte3:8;
	    unsigned int byte2:8;
	  } w;
	};

#endif

public:
	CBlowFish ();
	~CBlowFish ();

	void 		Initialize (BYTE key[], int keybytes);
	DWORD		GetOutputLength (DWORD lInputLong);
	DWORD		Encode (BYTE * pInput, BYTE * pOutput, DWORD lSize);
	void		Decode (BYTE * pInput, BYTE * pOutput, DWORD lSize);

private:
	DWORD 		* PArray;
	DWORD		(* SBoxes)[256];
	void 		Blowfish_encipher (DWORD *xl, DWORD *xr);
	void 		Blowfish_decipher (DWORD *xl, DWORD *xr);
};

#endif //BLOWFISH_H_INCLUDED_
