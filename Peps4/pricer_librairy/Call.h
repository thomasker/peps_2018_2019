#pragma once
#include "ExportDll.h"

#include "Produit.h"
#include "pnl/pnl_matrix.h"
#include "Date.h"
#include "TimeManager.h"
#include "sousjacent.h"
using namespace Outils;

namespace  Produits
{
	class PRICER_API  Call :public Produit
	{
	public:

		double GetPrice(PnlMat* paths, double r, int t);
		double GetPayOff(PnlMat* paths) ;
		PnlVectInt* GetDates();
		SousJacent* GetSousjacents();
		int GetSousjacentsSize();
		int GetNbForeignCurrency();
		Call(double K, date start, date end);
		~Call();

	private:
		double _K;
		PnlVectInt* dates;
		SousJacent sousJacents[1];
		int sousJacentsSize = 1;
		//int _T;
	};
}