using namespace std;
extern "C" {
  #include <stdint.h>
}

#include "FactoryNeuroMemAI.h"

#define HW_BRAINCARD 1 // neuron access through SPI_CS pin 10
#define HW_NEUROSHIELD 2 // neuron access through SPI_CS pin 7
#define HW_COGNISTAMP 3 // neuron access through SPI_CS pin 10
#define HW_NEUROTILE 4 // neuron access through SPI_CS pin 10
#define HW_COMPANION 6

NeuroMemAI* FactoryNeuroMemAI::GetHandlerNeuroMemAI(int platform) {
	NeuroMemAI* ret = NULL;
	if(platform == HW_COMPANION){
		ret = new STUNeuroMemAI();
	} else {
		ret = new NeuroMemAI();
	}
	return ret;
}