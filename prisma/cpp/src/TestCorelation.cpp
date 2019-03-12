
#include "TestCorelation.h"
#include "Corelation.h"
#include <iostream>
#include "pnl/pnl_random.h" 
#include <time.h>
using namespace Pricer;

namespace Test
{
	bool TestCorelation::TestTotal()
	{
		std::cout << "===== START TEST TOTAL CORELATION =====\n";
		bool isOK = true;
		if (!TestGetCorelation()) {
			std::cout << "Test Get Corelation---------- Not OK!\n";
			isOK = false;
		}
		else
		{
			std::cout << "Test Get Corelation OK!\n";
		}
		if (!TestGetMatCorelation()) {
			std::cout << "Test Get Mat Corelation---------- Not OK!\n";
			isOK = false;
		}
		else
		{
			std::cout << "Test Get Mat Corelation OK!\n";
		}
		return isOK;
	}
	bool TestCorelation::TestGetCorelation()
	{
		// Generate 2 G vect with rho correlation 
		double rho = 0.5;
		int n = 100;
		PnlRng* rng = pnl_rng_create(PNL_RNG_MERSENNE);
		pnl_rng_sseed(rng, (unsigned long)time(NULL));

		PnlVect * vectA = pnl_vect_create(n);
		PnlVect * vectB = pnl_vect_create(n);
		pnl_vect_rng_normal(vectA, n, rng);
		pnl_vect_rng_normal(vectB, n, rng);
		PnlVect*  vectC = pnl_vect_create(n);
		pnl_vect_clone(vectC, vectA);
		pnl_vect_mult_scalar(vectC, rho);
		pnl_vect_mult_scalar(vectB, sqrt(1-rho*rho));
		pnl_vect_plus_vect(vectC, vectB);

		std::cout << "la VectA \n";
		pnl_vect_print(vectA);
		std::cout << "la VectC \n";
		pnl_vect_print(vectC);

		PnlVect * vectTime = pnl_vect_create(n);
		PnlVect * pathsA = pnl_vect_create(n);
		PnlVect * pathsC = pnl_vect_create(n);
		/*GetPriceEvolution2(pathsA, vectA , vectTime , 0.1, 0.1, 100);
		GetPriceEvolution2(pathsC, vectC, vectTime, 0.1, 0.1, 100);*/
		double result = 0;
		//result= Corelation::GetCorelation(pathsA, pathsC);
		std::cout <<"la corelation est de :" << result << "\n";
		return false;
	}
	bool TestCorelation::TestGetMatCorelation()
	{
		/*GetMatCorelation(PnlMat * correlations, PnlMat * products)*/
			return false;
	}
}