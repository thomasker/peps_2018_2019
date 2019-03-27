#pragma once

#include "Produit.h"
#include "sousjacent.h"
#include "TimeManager.h"
#include "pnl/pnl_matrix.h"
#include <iostream> 
#include <map> 
#include <iterator> 
#include "sousjacent.h"

using namespace std;


namespace Produits
{
	class   Prisma :public Produit
	{
	public:
		double GetPrice(PnlMat* paths, double r, int t);
		double GetPayOff(PnlMat* paths);

		double Dividendes(double r, int day);

		PnlVectInt* GetDates();
		int GetMaturity();
		SousJacent* GetSousjacents();
		int GetSousjacentsSize();
		int GetNbForeignCurrency();
		int getDateIterateur(int date);
		Prisma();
		Prisma(PnlVectInt* _QuarterlyObservationDates, PnlVectInt* _AnnualObservationDates, PnlVectInt* _DividendesDates, PnlVectInt* _Dates);
		virtual ~Prisma();

		PnlVectInt* DividendesDates;
	private:
		int sousJacentsSize;
		double DividendAmont;
		double GuaranteedRefund;
		double Turnout;
		double FixedPremium;
		double MinimalPercentage;
		PnlVect* P0;


		// contien un des entiers , correspondant au dates d'evaluations, 
		//a remplire dinamiquement en fonction des date de debut et de fin , clin d oeil
		PnlVectInt* QuarterlyObservationDates;
		PnlVectInt* AnnualObservationDates;

		PnlVectInt* Dates;


		SousJacent sousJacents[16];

		void SousJacentsConstructor();
		map<int, int>* InversedDates = new map<int, int>();
	};
}
