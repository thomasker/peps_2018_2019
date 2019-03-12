#pragma once

#include "pnl/pnl_matrix.h"

namespace Pricer
{
	class  Corelation {
	public:
		static double GetCorelation(PnlVect* productA, PnlVect* productB);

		static void GetMatCorelation(PnlMat* correlations, PnlMat* products);
	};
}