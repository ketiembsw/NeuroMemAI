/************************************************************************/
/*																		
 *	NeuroMemSPI.h	--	SPI driver common to all NeuroMem hardware			        	
 *	Copyright (c) 2017, General Vision Inc, All rights reserved
 *
 */
/******************************************************************************/
#ifndef _STUNeuroMemSPI_h_
#define _STUNeuroMemSPI_h_

#include "NeuroMemSPI.h"

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

class STUNeuroMemSPI : public NeuroMemSPI
{
	private : 
		int ClockSpeed = SPI_CLOCK_DIV2;

	public:
		STUNeuroMemSPI();	
		int connect(int Platform);
		void SetClockSpeed(int speed) { ClockSpeed = speed; };
		int FPGArev();
		int read(unsigned char reg);
		void write_one(unsigned char reg, int data);
		void write_comp(int length, int data[]);
		void write_lcomp(int length, int data[]);
		void write_cat(int length, int data[], int category);
		void readAddr(long addr, int length, int data[]);
		unsigned char spi_trans_companion(unsigned char tx_data);
};
#endif
