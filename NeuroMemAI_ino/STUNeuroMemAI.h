/************************************************************************/
/*																		
 *	NeuroMemAI.h	--	Class to use a NeuroMem network			        	
 *	Copyright (c) 2018, General Vision Inc, All rights reserved	
 */
/******************************************************************************/
#ifndef _STUNeuroMemAI_h_
#define _STUNeuroMemAI_h_

extern "C" {
  #include <stdint.h>
}

#include "NeuroMemAI.h"

class STUNeuroMemAI : public NeuroMemAI
{
	public:		
		STUNeuroMemAI();
		int testCall();
		int begin(int Platform);
		void forget();
		void forget(int Maxif);
		void clearNeurons();
		int countNeuronsAvailable();
		
		void setContext(int context, int minif, int maxif);
		void getContext(int* context, int* minif, int* maxif);
		void setRBF();
		void setKNN();
		
		int broadcast(int vector[], int length);
		int learn(int vector[], int length, int category);
		int classify(int vector[], int length);
		int classify(int vector[], int length, int* distance, int* category, int* nid);
		int classify(int vector[], int length, int K, int distance[], int category[], int nid[]);

		void readNeuron(int nid, int model[], int* context, int* aif, int* category);
		void readNeuron(int nid, int neuron[]);
		int readNeurons(int neurons[]);
		void writeNeurons(int neurons[], int ncount);

		int CheckNMCompanion();
	
		//--------------------------
		// NeuroMem register access
		//
		// Use with caution
		// refer to the http://www.general-vision.com/documentation/TM_NeuroMem_Technology_Reference_Guide.pdf
		//
		//--------------------------
		int NCOUNT();
		void GCR(int value);
		int GCR();
		void MINIF(int value);
		int MINIF();
		void MAXIF(int value);
		int MAXIF();
		void NSR(int value);
		int NSR();
		void AIF(int value);
		int AIF();
		void CAT(int value);
		int CAT();
		void NID(int value);
		int DIST();
		void RESETCHAIN();
		void NCR(int value);
		int NCR();		
		void COMP(int value);
		int COMP();
		void LCOMP(int value);
		int LCOMP();
		//void TESTCAT(int value);
		//void TESTCOMP(int value);
		
		//-----------------------------------
		// Access to SD card
		//-----------------------------------
		// compatible with the NeuroMem knowledge Builder knowledge files
		int saveKnowledge_SDcard(char* filename);
		int loadKnowledge_SDcard(char* filename);
		
		// compatible with the Image Knowledge Builder projects
		int saveProject_SDcard(char* filename, int roiWidth, int roiHeight, int bWidth, int bHeight);
		int loadProject_SDcard(char* filename, int* roiWidth, int* roiHeight, int* bWidth, int* bHeight);
};
#endif
