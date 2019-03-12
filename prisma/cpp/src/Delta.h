#pragma once


#include "pnl/pnl_matrix.h"
#include "Prisma.h"

using namespace Produits;

namespace Pricer
{
	class  Delta {
	private:

	public:
		static void GetDelta(PnlVect* Delta, Prisma produit,PnlMat* prices, int n ,double r, int maturite, int t );

	};
}