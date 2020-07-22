#ifndef _FacrotyNeuroMemAI_h_
#define _FacrotyNeuroMemAI_h_

#include "STUNeuroMemAI.h"

extern "C" {
  #include <stdint.h>
}

class FactoryNeuroMemAI
{
	public:
		static NeuroMemAI* GetHandlerNeuroMemAI(int platform);
};
#endif
