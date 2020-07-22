/************************************************************************/
/*																		
 *	NeuroMemSPI.h	--	SPI driver common to all NeuroMem hardware			        	
 *	Copyright (c) 2017, General Vision Inc, All rights reserved
 *
 */
/******************************************************************************/
#ifndef _NeuroMemSPI_h_
#define _NeuroMemSPI_h_

#include "SPI.h"

extern "C" {
  #include <stdint.h>
}

// copy and paste in your sketch if using direct access to the NeuroMem chip registers
#define MOD_NM 0x01
#define NM_NCR 0x00
#define NM_COMP 0x01
#define NM_LCOMP 0x02
#define NM_DIST 0x03
#define NM_INDEXCOMP 0x03
#define NM_CAT 0x04
#define NM_AIF 0x05
#define NM_MINIF 0x06
#define NM_MAXIF 0x07
#define NM_TESTCOMP 0x08
#define NM_TESTCAT 0x09
#define NM_NID 0x0A
#define NM_GCR 0x0B
#define NM_RESETCHAIN 0x0C
#define NM_NSR 0x0D
#define NM_NCOUNT 0x0F	
#define NM_FORGET 0x0F

class NeuroMemSPI
{
	public:
		int SPISelectPin;
		int SPIspeed;
		int FPGAFlashPin = 8;

		NeuroMemSPI();
		int platform=0;		
		virtual int connect(int Platform);
		virtual int FPGArev();
		virtual int read (unsigned char mod, unsigned char reg);
		virtual void write(unsigned char mod, unsigned char reg, int data);
		virtual void writeAddr(long addr, int length, int data[]);
		virtual void readAddr(long addr, int length, int data[]);
		
};
#endif
