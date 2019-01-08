#pragma once
#include "ExportDll.h"

//#ifdef PRICER_EXPORTS  
//#define PRICER_API __declspec(dllexport)   
//#else  
//#define PRICER_API __declspec(dllimport)   
//#endif  

#include "pnl/pnl_random.h"

namespace Pricer
{
	class Volatility {
	private:
		static double methode1(PnlVect * historic);
		static double methode2(PnlVect * historic);
		static double methode3(PnlVect *historic, PnlVect *dates);
	public:
		PRICER_API static double historicVolatility (PnlVect* historic, PnlVect *dates);
		PRICER_API static void historicVolatility(PnlVect* volatility, PnlMat* historic, PnlVect *dates);
		//PRICER_API static double implicitVolatility(PnlVect* historic);

	};
}