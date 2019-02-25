#include "TestData.h"
#include "Data.h"
#include <iostream>
#include "pnl/pnl_random.h" 
#include "Call.h"
#include "Prisma.h"

using namespace Outils;
using namespace Produits;

namespace Test
{

	bool TestData::TestTotal()
	{
		std::cout << "===== START TEST TOTAL DATA =====\n";
		bool isOK = true;
		if (!TestSet_GetVolatilitySouJacent()) {
			std::cout << "Test Set Get Volatility SouJacent---------- Not OK!\n";
			isOK = false;
		}
		else
		{
			std::cout << "Test Set Get Volatility SouJacent OK!\n";
		}
		if (!TestSet_GetVolatilityCurency()) {
			std::cout << "Test Set Get Volatility Curency---------- Not OK!\n";
			isOK = false;
		}
		else
		{
			std::cout << "Test Set Get Volatility Curency OK!\n";
		}
		return isOK;
	}

	bool TestData::TestSet_GetVolatilitySouJacent()
	{
		double K = 1.;
		Call* produit = new Call(K, date(1, 1, 2002), date(2, 1, 2002));
		Data data = Data(produit);
		double vect[] = { 12. };
		PnlVect* volvect = pnl_vect_create_from_ptr(1, vect);
		data.SetVectVolatilitySouJacent(volvect, 0);
		PnlVect* Result = pnl_vect_create(1);
		data.GetHistoricAllVolatilitySouJacent(Result, 0);
		if (GET(Result,0)!= GET(volvect, 0)) {
			std::cout << "error Set and Get All basic\n";
			return false;
		}
		SousJacent toto = SousJacent();
		char name[] = "SousJacent";
		toto.setName(name);
		if (data.GetHistoricVolatilitySouJacent(toto, 0) != 12.) {
			std::cout << "error Set and Get one basic \n";
			return false;
		}

		Prisma* produit2 = new Prisma();
		Data data2 = Data(produit2);
		double vect2[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
		PnlVect* volvect2 = pnl_vect_create_from_ptr(15, vect2);
		data2.SetVectVolatilitySouJacent(volvect2, 0);

		data2.GetHistoricAllVolatilitySouJacent(Result, 0);
		pnl_vect_print(Result);
		if (Result->size != 15 || GET(Result, 0)!=1|| GET(Result, 14)!=15) {
			std::cout << "error Set and Get All Prisma\n";
			return false;
		}
		
		SousJacent* toto2  = produit2->GetSousjacents();
		if (data2.GetHistoricVolatilitySouJacent(toto2[0], 0) != 1|| data2.GetHistoricVolatilitySouJacent(toto2[5], 0) != 6) {
			std::cout << "error Set and Get one Prisma\n";
			return false;
		}
		
		
		return true;
	}

	bool TestData::TestSet_GetVolatilityCurency()
	{
		return false;
	}
}