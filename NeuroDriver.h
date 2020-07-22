#pragma once
#pragma once

#ifndef NEURO_DRIVER_H
#define NEURO_DRIVER_H
#include "./NeuroMemAI_ino/FactoryNeuroMemAI.h"

#define HW_NEUROSHIELD 2 // neuron access through SPI_CS pin 7
#define HW_COMPANION 6 // neuron access through SPI_CS pin 8

#define NM_CS_BRAINCARD 10
#define NM_CS_SUTFPGA 10

//#define NEURONSIZE 128 //memory capacity of each neuron in byte
//#define MAXNEURONS 128 //number of neurons per chip
#define SD_CHIP_NUMBER 6
#define UNSIGNED_CHAR_MAX 255
#define NEURO_MUSIC_FILE_NAME	"MS.DAT"
#define NEURO_NAVI_FILE_NAME	"NV.DAT"
#define NEURO_FILE_DEFULAT_NAME			"NR.DAT"
#define NEURO_FILE_INIT_NAME NEURO_MUSIC_FILE_NAME

class NeuroDriver {
public :
	enum EnumNeuroFileMode {
		EnumNeuroFileMode_Default,
		EnumNeuroFileMode_Music,
		EnumNeuroFileMode_Navi,
		EnumNeuroFileMode_END
	};
protected:	
	NeuroMemAI* hNN = NULL;
	int DEBUG_PRINT = 0;
	char file_name[256];
	EnumNeuroFileMode m_file_mode = EnumNeuroFileMode_Music;


public:
	int Neuro_board = 0;

	NeuroDriver() {
	}

	int init(int hw_platform, int max_if = 0x4000, int debug_print_val = 1, int sd_card_mode = 1) {
		if (!Serial) {
			return -1;
		}
		Neuro_board = hw_platform;
		DEBUG_PRINT = debug_print_val;
		hNN = FactoryNeuroMemAI::GetHandlerNeuroMemAI(hw_platform);
		if (hNN == NULL) {
			Serial.println("GetHandlerNeuroMemAI return NULL pointer!");
			return -1;
		}
		else {
			Serial.print("hw_platform ["); Serial.print(hNN->testCall());
			Serial.println("] is initialized.");
		}


		//hNN init
		if ( hNN->begin(hw_platform) == 0 )
		{
			Serial.print("\nYour NeuroMem_Smart device is initialized! ");
			Serial.print("\nThere are "); Serial.print(hNN->navail); Serial.print(" neurons\n");
		}
		else
		{
			Serial.print("\nYour NeuroMem_Smart device is NOT found!");
			Serial.print("\nCheck your device type and connection.\n");
			return -1;
		}
		
		hNN->forget(max_if);
		if (sd_card_mode == 1) {
			changeNeuroFile(m_file_mode);
			loadNeuroFile();
		}

		return 0;
	}

	int getNeuronAvailable() {
		return hNN->countNeuronsAvailable();
	}
	int getNeuronCommitted() {
		return hNN->NCOUNT();
	}

	int convert_vector(float in_vector[], int out_vector[], int start_index, int in_length, int out_length, float max) {
		int compresing_unit = int(in_length / out_length);
		int index_in_vector = 0;
		int index_out_vector = start_index;
		while (index_in_vector < in_length) {
			int sum = 0;
			for (int i = 0  ; i < compresing_unit;i++) {
				sum += in_vector[index_in_vector];
				index_in_vector++;
			}
			int mean = (int)(sum / compresing_unit);
			out_vector[index_out_vector] = (unsigned char)(mean * UNSIGNED_CHAR_MAX / max);
			index_out_vector++;
		}
		return 1;
	}

	//return NM_NCOUNT
	int learn(int vector[], int length, int category) {
		return hNN->learn(vector, length, category);
	}

	//return NM_NSR
	int classify(int vector[], int length, int *distance, int *category, int *nid) {
		int ret = hNN->classify(vector, length, distance, category, nid);
		if (ret == 8) {
			return true;
		}
		return false;
		
	}

	void displayNeurons(int displayLength)
	{
		int ncr, cat, aif, ncount;
		int model[256];
		ncount = hNN->NCOUNT();
		if (ncount == 0xFFFF) ncount = hNN->navail;
		Serial.print("\n\nDisplay the neurons, count="); Serial.print(ncount);
		for (int i = 0; i<ncount; i++)
		{
			hNN->readNeuron(i, model, &ncr, &aif, &cat);
			Serial.print("\nneuron#"); Serial.print(i); Serial.print("\tmodel=");
			for (int k = 0; k<displayLength; k++) { Serial.print(model[k]); Serial.print(", "); }
			Serial.print("\tncr="); Serial.print(ncr);
			Serial.print("\taif="); Serial.print(aif);
			Serial.print("\tcat="); Serial.print(cat);
		}
	}

	void DisplayNeuroCount() {
		Serial.print("Knowledge count : "); Serial.println(getNeuronCommitted());
	}

	enum EnumNeuroFileStore {
		EnumNeuroFileMode_Save,
		EnumNeuroFileMode_Load
	};
	void DisplaySDCardErrorMsg(EnumNeuroFileStore mode, int error, bool debug_print = true) {
		switch (error)
		{
		case 1:
			Serial.println("\nSD card not found");
			break;
		case 2:
			Serial.println("\nFile not found");
			break;
		case 3:
			Serial.println("\nFile corrupted");
			break;
		case 4:
			Serial.println("\nIncorrect format");
			break;
		case 5:
			Serial.println("\nKnowledge cannot fit, Insuficient neuron memory size");
			break;
		case 6:
			Serial.println("\nKnowledge cannot fit, Insuficient number of neurons");
			break;
		default:
			if (debug_print) {
				if (mode == EnumNeuroFileMode_Load) {
					Serial.print("Knowledge is loaded by reading "); Serial.println(file_name);
					DisplayNeuroCount();
				}
				else if (mode == EnumNeuroFileMode_Save) {
					Serial.print("Knowledge is saved by writing "); Serial.println(file_name);
					DisplayNeuroCount();
				}
			}			
			break;
		}
	}

	EnumNeuroFileMode getNeuroFileMode() {
		return m_file_mode;
	}

	void changeNeuroFile(EnumNeuroFileMode file_mode, bool debug_print = true) {
		m_file_mode = file_mode;
		switch (file_mode)
		{
		case EnumNeuroFileMode::EnumNeuroFileMode_Music :
			strcpy(file_name, NEURO_MUSIC_FILE_NAME);
			break;
		case EnumNeuroFileMode::EnumNeuroFileMode_Navi :
			strcpy(file_name, NEURO_NAVI_FILE_NAME);
			break;
		case EnumNeuroFileMode::EnumNeuroFileMode_Default :
		default:
			strcpy(file_name, NEURO_FILE_DEFULAT_NAME);
			break;
		}
		if (debug_print) {
			Serial.print("Neuro vector group : "); Serial.println(file_name);
		}		
	}

	void saveNeuroFile(bool debug_print = true) {
		DisplaySDCardErrorMsg(EnumNeuroFileMode_Save, hNN->saveKnowledge_SDcard(file_name), debug_print);
	}

	void loadNeuroFile(bool debug_print = true) {
		DisplaySDCardErrorMsg(EnumNeuroFileMode_Load, hNN->loadKnowledge_SDcard(file_name), debug_print);
	}

	void ClearNeuro(int max_if = 0x4000) {
		hNN->forget(max_if);
		DisplayNeuroCount();
	}

	int CheckNMCompanion() {
		if (Neuro_board == HW_COMPANION) {
			((STUNeuroMemAI*)hNN)->CheckNMCompanion();
		}		
		return 0;
	}
};

#endif //NEURO_DRIVER_H