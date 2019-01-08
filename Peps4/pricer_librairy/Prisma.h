#pragma once

#include "ExportDll.h"

#include "Produit.h"
#include "sousjacent.h"
#include "TimeManager.h"
#include "pnl/pnl_matrix.h"
#include <iostream> 
#include <list> 
#include <iterator> 
#include "sousjacent.h"

using namespace std;

namespace Produits
{
	class  PRICER_API  Prisma :public Produit
	{
	public:
		double GetPrice(PnlMat* paths, double r,int t);

		double GetPayOff(PnlMat* paths) ;

		double Dividendes(double r, int day);

		PnlVectInt* GetDates();
		int GetMaturity();
		SousJacent* GetSousjacents();
		int GetSousjacentsSize();
		int GetNbForeignCurrency();
		Prisma();
		Prisma(PnlVectInt* _QuarterlyObservationDates, PnlVectInt* _AnnualObservationDates, PnlVectInt* _DividendesDates, PnlVectInt* _Dates);
		virtual ~Prisma();

	private:
		void SousJacentsConstructor();
		SousJacent sousJacents[15];
		int sousJacentsSize = 15;
		double DividendAmont = 6;
		double GuaranteedRefund = 70.;
		double Turnout = 0.2;
		double FixedPremium = 8.;
		// contien un des entiers , correspondant au dates d'evaluations, 
		//a remplire dinamiquement en fonction des date de debut et de fin , clin d oeil
		PnlVectInt* QuarterlyObservationDates;
		PnlVectInt* AnnualObservationDates;
		PnlVectInt* DividendesDates;
		PnlVectInt* Dates;
	};
}
