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

#include "NeuroMemSPI.h"

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
#define NM_CS_SUTFPGA 8

// SPI speed
#define CK_BRAINCARD 4000000 // 4Mhz
#define CK_NEUROSHIELD 2000000 // 2 Mhz
#define CK_COGNISTAMP 2000000 // 2 Mhz
#define CK_NEUROTILE 4000000 // 4Mhz
#define CK_SUTFPGA 1000000 // 1 Mhz

#define SPI_CLK_DIV	SPI_CLOCK_DIV8


NeuroMemSPI::NeuroMemSPI(){	
}
// ------------------------------------------------------------ 
// Initialize the SPI communication and verify proper interface
// to the NeuroMem network by reading the default Minif value of 2-bytes
// return an error=1 otherwise
// ------------------------------------------------------------ 
int NeuroMemSPI::connect(int Platform)
{
	platform=Platform;
	SPI.begin();
	switch(platform)
	{
		case HW_NEUROSHIELD:
			SPI.setClockDivider(SPI_CLOCK_DIV8);
			SPISelectPin = NM_CS_NEUROSHIELD;
			SPIspeed= CK_NEUROSHIELD;	
			pinMode (SPISelectPin, OUTPUT);
			digitalWrite(SPISelectPin, HIGH);
			pinMode (5, OUTPUT); //pin Arduino_CON
			digitalWrite(5, LOW); // pin Arduino_CON
			pinMode (6, OUTPUT); //pin Arduino_SD_CS
			digitalWrite(6,HIGH); // pin Arduino_SD_CS
			delay(100);
			break;
		case HW_COGNISTAMP:
			SPI.setClockDivider(SPI_CLOCK_DIV8);
			SPISelectPin = NM_CS_COGNISTAMP;
			SPIspeed= CK_COGNISTAMP;	
			pinMode (SPISelectPin, OUTPUT);
			digitalWrite(SPISelectPin, HIGH);
			delay(100);
			break;
		case HW_BRAINCARD:
			SPISelectPin = NM_CS_BRAINCARD;
			SPIspeed= CK_BRAINCARD;
			// SPISelectPin and FPGAFlashPin must toggle together
			FPGAFlashPin=8;
			pinMode (SPISelectPin, OUTPUT);
			pinMode (FPGAFlashPin, OUTPUT);	// Using FPGA Flash pin
			digitalWrite(SPISelectPin, HIGH);
			digitalWrite(FPGAFlashPin, HIGH);
			// Reset by pulling both CSn low and returning to high
			digitalWrite(SPISelectPin, LOW);
			digitalWrite(FPGAFlashPin, LOW);
			delay(200);
			digitalWrite(SPISelectPin, HIGH);
			digitalWrite(FPGAFlashPin, HIGH);
			delay(500);
			break;
		case HW_NEUROTILE: 
			SPISelectPin = NM_CS_NEUROTILE;
			SPIspeed= CK_NEUROTILE;
			pinMode (SPISelectPin, OUTPUT);
			digitalWrite(SPISelectPin, HIGH);
			// Reset by pulling both CSn low and returning to high
			digitalWrite(SPISelectPin, LOW);
			delay(200);
			digitalWrite(SPISelectPin, HIGH);
			delay(500);
			break;
	} 
	// If NM chip present and SPI comm successful
	// Read MINIF (reg 6) and verify that it is equal to 
	Serial.println("NeuroMemSPI::connect");
	if(read(1, 6)==2)return(0);else return(1); 
}
// --------------------------------------------------------
// Read the FPGA_version
//---------------------------------------------------------
int NeuroMemSPI::FPGArev()
{
	int FPGArev=0;
	switch(platform)
	{
		case HW_BRAINCARD: FPGArev=read(1, 0x0E); break;
		case HW_NEUROSHIELD: FPGArev=read(2, 1); break;
		case HW_NEUROTILE: FPGArev=0; break;
	}
	return(FPGArev);
}
// --------------------------------------------------------
// SPI Read a register of the NeuroMem network
//---------------------------------------------------------
int NeuroMemSPI::read(unsigned char mod, unsigned char reg)
{
	//SPI.beginTransaction(SPISettings(SPIspeed, MSBFIRST, SPI_MODE0));
	SPI.setClockDivider(SPI_CLOCK_DIV8);
	digitalWrite(SPISelectPin, LOW);
	SPI.transfer(1);  // Dummy for ID
	SPI.transfer(mod);
	SPI.transfer(0);
	SPI.transfer(0);
	SPI.transfer(reg);
	SPI.transfer(0); // length[23-16]
	SPI.transfer(0); // length [15-8]
	SPI.transfer(1); // length [7-0]
	int data = SPI.transfer(0); // Send 0 to push upper data out
	data = (data << 8) + SPI.transfer(0); // Send 0 to push lower data out
	digitalWrite(SPISelectPin, HIGH);
	//SPI.endTransaction();
	return(data);
}
// ---------------------------------------------------------
// SPI Write a register of the NeuroMem network
// ---------------------------------------------------------
void NeuroMemSPI::write(unsigned char mod, unsigned char reg, int data)
{
	//SPI.beginTransaction(SPISettings(SPIspeed, MSBFIRST, SPI_MODE0));
	SPI.setClockDivider(SPI_CLOCK_DIV8);
	digitalWrite(SPISelectPin, LOW);
	SPI.transfer(1);  // Dummy for ID
	SPI.transfer(mod + 0x80); // module and write flag
	SPI.transfer(0);
	SPI.transfer(0);
	SPI.transfer(reg);
	SPI.transfer(0); // length[23-16]
	SPI.transfer(0); // length[15-8]
	SPI.transfer(1); // length[7-0]
	SPI.transfer((unsigned char)(data >> 8)); // upper data
	SPI.transfer((unsigned char)(data & 0x00FF)); // lower data
	digitalWrite(SPISelectPin, HIGH);
	//SPI.endTransaction();
}
// ---------------------------------------------------------
// SPI Write_Addr command
// multiple write of data in word format
// length is expressed in words
// ---------------------------------------------------------
void NeuroMemSPI::writeAddr(long addr, int length, int data[])
{
	//SPI.beginTransaction(SPISettings(SPIspeed, MSBFIRST, SPI_MODE0));
	SPI.setClockDivider(SPI_CLOCK_DIV8);
	digitalWrite(SPISelectPin, LOW);
	SPI.transfer(1);  // Dummy for ID
	SPI.transfer((byte)(((addr & 0xFF000000) >> 24) + 0x80)); // Addr3 and write flag
	SPI.transfer((byte)((addr & 0x00FF0000) >> 16)); // Addr2
	SPI.transfer((byte)((addr & 0x0000FF00) >> 8)); // Addr1
	SPI.transfer((byte)(addr & 0x000000FF)); // Addr0
	SPI.transfer((byte)((length & 0x00FF0000) >> 16)); // Length2
	SPI.transfer((byte)(length & 0x0000FF00) >> 8); // Length1
	SPI.transfer((byte)(length & 0x000000FF)); // Length 0
	for (int i = 0; i < length; i++)
	{
		SPI.transfer((data[i] & 0xFF00)>> 8);
		SPI.transfer(data[i] & 0x00FF);
	}
	digitalWrite(SPISelectPin, HIGH);
	//SPI.endTransaction();
} 
//---------------------------------------------
// SPI Read_Addr command
// multiple read of data in word format
// length is expressed in words
//---------------------------------------------
void NeuroMemSPI::readAddr(long addr, int length, int data[])
{
	//SPI.beginTransaction(SPISettings(SPIspeed, MSBFIRST, SPI_MODE0));
	SPI.setClockDivider(SPI_CLOCK_DIV8);
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
	//SPI.endTransaction();
}

