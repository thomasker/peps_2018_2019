#pragma once

#include "pnl/pnl_matrix.h"
#include "Model.h"

namespace Models
{
	class  BlackScholeModel : public GenericModel
	{
	public:
		 double GetPriceMatutity(PnlVect* W, double t, double r, double theta, double S0);

		 void GetPriceEvolution1(PnlVect* paths ,PnlVect* W, PnlVectInt* tvect, double r, double thetha, double S0, int t0 = 0);
		 void GetPriceEvolution2(PnlVect* paths, PnlVect* G, PnlVectInt* tvect, double r, double thetha, double S0, int t0 = 0);

		 double GetNextPrice1(PnlVect* W, PnlVectInt* tvect, int nt, double r, double thetha, double St);
		 double GetNextPrice2(double G, PnlVectInt* tvect, int nt, double r, double thetha, double St);

		 void GetMatPrices(PnlMat * paths, PnlMat* G, PnlVectInt* tvect, double r, double thetha, double S0, int t0 = 0);
	private:

	};
}