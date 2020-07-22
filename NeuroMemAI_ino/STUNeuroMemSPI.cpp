/************************************************************************/
/*																		
 *	NeuroMemSPI.cpp	--	SPI driver common to all NeuroMem hardware				        	
 *	Copyright (c) 2017, General Vision Inc, All rights reserved
 *
 *  Created September 8, 2017
 *  Modified 02/13/2018
 *  Support BrainCard, NeuroShield, CogniStamp, NeuroTile
 *  
 *  Does not use BeginTransaction and EndTransaction
 *  but rather the old setClcokDivider to remain compatible
 *  with the nepes OrangeBoard and other older Arduino controller
 *
 * http://www.general-vision.com/documentation/TM_NeuroMem_Smart_protocol.pdf
 */
/******************************************************************************/

/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include "STUNeuroMemSPI.h"

using namespace std;
extern "C" {
  #include <stdint.h>
}

// Hardware ID code
#define HW_BRAINCARD 1 // neuron access through SPI_CS pin 10
#define HW_NEUROSHIELD 2 // neuron access through SPI_CS pin 7
#define HW_COGNISTAMP 3 // neuron access through SPI_CS pin 10
#define HW_NEUROTILE 4 // neuron access through SPI_CS pin 10
#define HW_COMPANION 6

// SPI chip select pin
#define NM_CS_BRAINCARD 10
#define NM_CS_NEUROSHIELD 7
#define NM_CS_COGNISTAMP 10
#define NM_CS_NEUROTILE 10
#define NM_CS_COMPANION 7

// SPI speed
#define CK_BRAINCARD 4000000 // 4Mhz
#define CK_NEUROSHIELD 2000000 // 2 Mhz
#define CK_COGNISTAMP 2000000 // 2 Mhz
#define CK_NEUROTILE 4000000 // 4Mhz
#define CK_SUTFPGA 1000000 // 1 Mhz
#define CK_COMPANION 8000000 // 8Mhz

#define SPI_CLK_DIV	SPI_CLOCK_DIV8

STUNeuroMemSPI::STUNeuroMemSPI(){
}
// ------------------------------------------------------------ 
// Initialize the SPI communication and verify proper interface
// to the NeuroMem network by reading the default Minif value of 2-bytes
// return an error=1 otherwise
// ------------------------------------------------------------ 
int STUNeuroMemSPI::connect(int Platform)
{
	platform=Platform;
	SPI.begin();
	switch(platform)
	{
		case HW_NEUROSHIELD:
		case HW_COGNISTAMP:
		case HW_BRAINCARD:
		case HW_NEUROTILE: 
		return (-1);
			break;
		case HW_COMPANION:
			SPISelectPin = NM_CS_COMPANION;
			SPIspeed = CK_COMPANION;
			pinMode(SPISelectPin, OUTPUT);
			digitalWrite(SPISelectPin, HIGH);
			SPI.setDataMode(SPI_MODE3); // CPOL = 1, CPHA = 1
			SPI.setBitOrder(MSBFIRST);
			break;
	} 
	// If NM chip present and SPI comm successful
	// Read MINIF (reg 6) and verify that it is equal to
	Serial.println("STUNeuroMemSPI::connect");
	int test_try = 10;
	int test_cnt = 0;
	for (int i = 0; i < test_try;i++) {
		if (read(6) == 2) test_cnt++;
	}
	if(test_try == test_cnt)return(0);
	else return(1); 
}

// For companion board - Transmit data
unsigned char STUNeuroMemSPI::spi_trans_companion(unsigned char tx_data)
{
	unsigned char rx_data = 0;
	digitalWrite(NM_CS_COMPANION, LOW);
	rx_data = (unsigned char)SPI.transfer(tx_data);
	digitalWrite(NM_CS_COMPANION, HIGH);
	return rx_data;
}

// --------------------------------------------------------
// Read the FPGA_version
//---------------------------------------------------------
int STUNeuroMemSPI::FPGArev()
{
	int FPGArev=0;
	return(FPGArev);
}

int STUNeuroMemSPI::read(unsigned char reg)
{
	SPI.setClockDivider(ClockSpeed);
	spi_trans_companion(128 + reg); // 8'b1xx0_0000 | REG[4:0]
	int data = spi_trans_companion(0); // Send 0 to push upper data out
	data = (data << 8) + spi_trans_companion(0); // Send 0 to push lower data out
	return(data);
}

void STUNeuroMemSPI::write_one(unsigned char reg, int data)
{
	SPI.setClockDivider(ClockSpeed);
	spi_trans_companion(reg);
	spi_trans_companion((unsigned char)(data >> 8)); // upper data
	spi_trans_companion((unsigned char)(data & 0x00FF)); // lower data
}
// ---------------------------------------------------------
// SPI Write a register of the NeuroMem network
// Write components
// ---------------------------------------------------------
void STUNeuroMemSPI::write_comp(int length, int data[])
{
	SPI.setClockDivider(ClockSpeed);
	spi_trans_companion(32); // 8'b001x_xxxx
	spi_trans_companion((unsigned char)(length)); // data length
	for (int i = 0; i < (unsigned char)(length); i++)
	{
		spi_trans_companion((unsigned char)(data[i]));
	}
}
// ---------------------------------------------------------
// SPI Write a register of the NeuroMem network
// Write components & Last component
// ---------------------------------------------------------
void STUNeuroMemSPI::write_lcomp(int length, int data[])
{
	SPI.setClockDivider(ClockSpeed);
	spi_trans_companion(64); // 8'b010x_xxxx
	spi_trans_companion((unsigned char)(length-1)); // data length - 1
	for (int i = 0; i < (unsigned char)(length-1); i++)
	{
		spi_trans_companion((unsigned char)(data[i]));
	}
	spi_trans_companion((unsigned char)(data[length-1]));
}
// ---------------------------------------------------------
// SPI Write a register of the NeuroMem network
// Write components & Last component & Category
// ---------------------------------------------------------
void STUNeuroMemSPI::write_cat(int length, int data[], int category)
{
	SPI.setClockDivider(ClockSpeed);
	spi_trans_companion(96); // 8'b011x_xxxx
	spi_trans_companion((unsigned char)(length - 1)); // data length - 1
	for (int i = 0; i < (unsigned char)(length - 1); i++)
	{
		spi_trans_companion((unsigned char)(data[i]));
	}
	spi_trans_companion((unsigned char)(data[length - 1]));
	spi_trans_companion((unsigned char)((category & 0x7F00) >> 8)); // upper cat
	spi_trans_companion((unsigned char)(category & 0x00FF)); // lower cat
}

void STUNeuroMemSPI::readAddr(long addr, int length, int data[])
{
	SPI.setClockDivider(ClockSpeed);
	digitalWrite(SPISelectPin, LOW);
	SPI.transfer(1);  // Dummy for ID
	SPI.transfer((byte)((addr & 0xFF000000) >> 24)); // Addr3 and write flag
	SPI.transfer((byte)((addr & 0x00FF0000) >> 16)); // Addr2
	SPI.transfer((byte)((addr & 0x0000FF00) >> 8)); // Addr1
	SPI.transfer((byte)(addr & 0x000000FF)); // Addr0
	SPI.transfer((byte)((length & 0x00FF0000) >> 16)); // Length2
	SPI.transfer((byte)(length & 0x0000FF00) >> 8); // Length1
	SPI.transfer((byte)(length & 0x000000FF)); // Lenght0
	for (int i = 0; i < length; i++)
	{
		data[i] = SPI.transfer(0); // Send 0 to push upper data out
		data[i] = (data[i] << 8) + SPI.transfer(0); // Send 0 to push lower data out
	}
	digitalWrite(SPISelectPin, HIGH);
}

