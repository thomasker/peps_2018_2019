#include "Basket.h"
#include <math.h>


namespace Produits
{
	double Basket::GetPrice(PnlMat * paths, double r, int t)
	{
		return  GetPayOff(paths)*exp(-r * (_T - t));
	}
	double Basket::GetPayOff(PnlMat * paths)
	{
		double St = 0.;
		for (int i = 0; i < sousJacentsSize; i++) {
			St+= MGET(paths, i, paths->n - 1);
		}
		St /= sousJacentsSize;
		return St < _K ? 0 : St - _K;
	}
	PnlVectInt * Basket::GetDates()
	{
		return dates;
	}
	SousJacent * Basket::GetSousjacents()
	{
		return sousJacents;
	}
	int Basket::GetSousjacentsSize()
	{
		return sousJacentsSize;
	}
	int Basket::GetNbForeignCurrency()
	{
		return 0;
	}
	Basket::Basket(double K, date start, date end)
	{
		_K = K;
		
		dates = pnl_vect_int_create(1);
		TimeManager::fillOpenDates(dates, start, end);
		_T = (int)pnl_vect_int_get(dates, dates->size - 1);
		

		SousJacentsConstructor();
	}
	Basket::~Basket()
	{
	}
	void Basket::SousJacentsConstructor()
	{

		//SousJacent sousjacent;
		char name[] = "Basket 0";
		//sousjacent.monaie = EUR;
		//sousjacent.setName(name);
		sousJacents[0] = SousJacent(name, EUR);

		
		char name1[] = "Basket 1";
		//sousjacent.setName(name1);
		sousJacents[1] = SousJacent(name1, EUR);

		char name2[] = "Basket 2";
		//sousjacent.setName(name2);
		sousJacents[2] = SousJacent(name2, EUR);

		char name3[] = "Basket 3";
		//sousjacent.setName(name3);
		sousJacents[3] = SousJacent(name3, EUR);

		char name4[] = "Basket 4";
		//sousjacent.setName(name4);
		sousJacents[4] = SousJacent(name4, EUR);

		char name5[] = "Basket 5";
		//sousjacent.setName(name5);
		sousJacents[5] = SousJacent(name5, EUR);

		char name6[] = "Basket 6";
		//sousjacent.setName(name6);
		sousJacents[6] = SousJacent(name6, EUR);

		char name7[] = "Basket 7";
		//sousjacent.setName(name7);
		sousJacents[7] = SousJacent(name7, EUR);

		char name8[] = "Basket 8";
		//sousjacent.setName(name8);
		sousJacents[8] = SousJacent(name8, EUR);

		char name9[] = "Basket 9";
		//sousjacent.setName(name9);
		sousJacents[9] = SousJacent(name9, EUR);

	}
}