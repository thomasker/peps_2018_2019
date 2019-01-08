#pragma once
#include "ExportDll.h"

#include "pnl/pnl_matrix.h"
#include "Prisma.h"

using namespace Produits;

namespace Pricer
{
	class PRICER_API Delta {
	private:

	public:
		static void GetDelta(PnlVect* Delta, Prisma produit,PnlMat* prices, int n ,double r, int maturite, int t );

	};
}