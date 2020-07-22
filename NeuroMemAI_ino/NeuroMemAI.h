/************************************************************************/
/*																		
 *	NeuroMemAI.h	--	Class to use a NeuroMem network			        	
 *	Copyright (c) 2018, General Vision Inc, All rights reserved	
 */
/******************************************************************************/
#ifndef _NeuroMemAI_h_
#define _NeuroMemAI_h_

#include "NeuroMemSPI.h"

extern "C" {
  #include <stdint.h>
}

class NeuroMemAI
{
	public:
				
		static const int NEURONSIZE=256; //memory capacity of each neuron in byte		
		static const int KN_FORMAT=0x1704; // version number for the save neuron file format
		int navail=0; // initialized during the begin function
		
		NeuroMemAI();
		int testCall();
		virtual int begin(int Platform);
		virtual void forget();
		virtual void forget(int Maxif);
		virtual void clearNeurons();
		virtual int countNeuronsAvailable();
		
		virtual void setContext(int context, int minif, int maxif);
		virtual void getContext(int* context, int* minif, int* maxif);
		virtual void setRBF();
		virtual void setKNN();
		
		virtual int broadcast(int vector[], int length);
		virtual int learn(int vector[], int length, int category);
		virtual int classify(int vector[], int length);
		virtual int classify(int vector[], int length, int* distance, int* category, int* nid);
		virtual int classify(int vector[], int length, int K, int distance[], int category[], int nid[]);

		virtual void readNeuron(int nid, int model[], int* context, int* aif, int* category);
		virtual void readNeuron(int nid, int neuron[]);
		virtual int readNeurons(int neurons[]);
		virtual void writeNeurons(int neurons[], int ncount);
	
		//--------------------------
		// NeuroMem register access
		//
		// Use with caution
		// refer to the http://www.general-vision.com/documentation/TM_NeuroMem_Technology_Reference_Guide.pdf
		//
		//--------------------------
		virtual int NCOUNT();
		virtual void GCR(int value);
		virtual int GCR();
		virtual void MINIF(int value);
		virtual int MINIF();
		virtual void MAXIF(int value);
		virtual int MAXIF();
		virtual void NSR(int value);
		virtual int NSR();
		virtual void AIF(int value);
		virtual int AIF();
		virtual void CAT(int value);
		virtual int CAT();
		virtual void NID(int value);
		virtual int DIST();
		virtual void RESETCHAIN();
		virtual void NCR(int value);
		virtual int NCR();
		virtual void COMP(int value);
		virtual int COMP();
		virtual void LCOMP(int value);
		virtual int LCOMP();
		//void TESTCAT(int value);
		//void TESTCOMP(int value);
		
		//-----------------------------------
		// Access to SD card
		//-----------------------------------
		int SD_select=0;
		bool SD_detected=false;
		// compatible with the NeuroMem knowledge Builder knowledge files
		virtual int saveKnowledge_SDcard(char* filename);
		virtual int loadKnowledge_SDcard(char* filename);
		
		// compatible with the Image Knowledge Builder projects
		virtual int saveProject_SDcard(char* filename, int roiWidth, int roiHeight, int bWidth, int bHeight);
		virtual int loadProject_SDcard(char* filename, int* roiWidth, int* roiHeight, int* bWidth, int* bHeight);
};
#endif
