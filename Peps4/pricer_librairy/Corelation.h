#pragma once
#include "ExportDll.h"

#include "pnl/pnl_matrix.h"

namespace Pricer
{
	class PRICER_API Corelation {
	public:
		static double GetCorelation(PnlVect* productA, PnlVect* productB);

		static void GetMatCorelation(PnlMat* correlations, PnlMat* products);
	};
}