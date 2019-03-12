

#include <iostream>
#include "TestProduit.h"

namespace Test
{
	bool TestProduit::TestTotal()
	{
		std::cout << "===== START TEST TOTAL PRODUIT =====\n";
		bool isOK = true;
		if (!TestSouJacent()) {
			std::cout << "Test SouJacent---------- Not OK!\n";
			isOK = false;
		}
		else
		{
			std::cout << "Test SouJacent OK!\n";
		}

		if (!TestGetPayOff_Call()) {
			std::cout << "Test Get PayOff Call---------- Not OK!\n";
			isOK = false;
		}
		else
		{
			std::cout << "Test Get PayOff Call OK!\n";
		}

		if (!TestGetPayOff_Prisma()) {
			std::cout << "Test Get PayOff Prisma---------- Not OK!\n";
			isOK = false;
		}
		else
		{
			std::cout << "Test Get PayOff Prisma OK!\n";
		}

		if (!TestGetPrice_Prisma()) {
			std::cout << "Test Get Price Prisma---------- Not OK!\n";
			isOK = false;
		}
		else
		{
			std::cout << "Test Get Price Prisma OK!\n";
		}

		if (!TestDividendes_Prisma()) {
			std::cout << "Test Dividendes Prisma---------- Not OK!\n";
			isOK = false;
		}
		else
		{
			std::cout << "Test Dividendes Prisma OK!\n";
		}

		if (!TestConstructeur_Prisma()) {
			std::cout << "Test Constructeur Prisma---------- Not OK!\n";
			isOK = false;
		}
		else
		{
			std::cout << "Test Constructeur Prisma OK!\n";
		}
		return isOK;
	}

	bool TestProduit::TestSouJacent()
	{
		
		SousJacent sj= SousJacent();
		char entrer[] = "toto";
		sj.setName(entrer);
		const char* Sortie = sj.getName();

		if (*entrer != *Sortie) {
			std::cout << "error set/get name basic !\n";
			return false;
		}
		
		return true;
	}

	bool TestProduit::TestGetPayOff_Call()
	{
		double K = 110.0;
		double ST = K+10;
		/*int T = 1;*/
		Call produit = Call(K, date(1,1,2002),date(2,1,2002));
		PnlMat* paths = pnl_mat_create(1,1);
		pnl_mat_set(paths,0,0, ST);
		double result =produit.GetPayOff(paths);
		if (result != ST - K) {
			std::cout << "error Call Get PayOff K<ST !\n";
			return false;
		}
		ST = K;
		pnl_mat_set(paths, 0, 0, K);
		result = produit.GetPayOff(paths);
		if (result != 0) {
			std::cout << "error Call Get PayOff  K=ST !\n";
			return false;
		}
		ST = K-10;
		pnl_mat_set(paths, 0, 0, K);
		result = produit.GetPayOff(paths);
		if (result != 0) {
			std::cout << "error Call Get PayOff K>ST !\n";
			return false;
		}
		return true;
	}

	bool TestProduit::TestGetPayOff_Prisma()
	{
		double ST = 110;
		PnlVectInt* dateQuart = pnl_vect_int_create(1);
		PnlVectInt* dateAnuel = pnl_vect_int_create(1);
		PnlVectInt* dateDividend = pnl_vect_int_create(1);
		int day[] = { 1,2,3,4,5 };
		PnlVectInt* date = pnl_vect_int_create_from_ptr(5, day);
		Prisma produit = Prisma(dateQuart, dateAnuel, dateDividend, date);

		

		PnlMat* paths = pnl_mat_create(1, 1);
		pnl_mat_set(paths, 0, 0, ST);

		produit.GetPayOff(paths);

		return false;
	}
	bool TestProduit::TestGetPrice_Prisma()
	{
		return false;
	}
	bool TestProduit::TestDividendes_Prisma()
	{
		return false;
	}
	bool TestProduit::TestConstructeur_Prisma()
	{
		return false;
	}
	
}