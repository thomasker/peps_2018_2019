#pragma once


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
		static double methode3(PnlVect *historic, PnlVectInt *dates);
	public:
		static double historicVolatility(PnlVect* historic, PnlVectInt *dates);
		static void historicVolatility(PnlVect* volatility, PnlMat* historic, PnlVectInt *dates);
		// static double implicitVolatility(PnlVect* historic);

	};
}