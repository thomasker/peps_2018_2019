#pragma once

#include "ExportDll.h"

//#ifdef MODELS_EXPORTS  
//#define MODELS_API __declspec(dllexport)   
//#else  
//#define MODELS_API __declspec(dllimport)   
//#endif  

namespace Models
{
	class PRICER_API GenericModel {
	public:
		double GetPriceMatutity();

	};
}