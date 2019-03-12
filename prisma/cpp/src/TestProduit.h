#pragma once


#include "Produit.h"
#include "sousjacent.h"
#include "Call.h"
#include "Prisma.h"

using namespace Produits;

namespace Test
{
	class TestProduit
	{
	public:
		static bool TestTotal();

		static bool TestSouJacent();

		static bool TestGetPayOff_Call();
		static bool TestGetPrice_Call();

		static bool TestGetPayOff_Prisma();
		static bool TestGetPrice_Prisma();
		static bool TestDividendes_Prisma();
		static bool TestConstructeur_Prisma();

	private:

	};
}