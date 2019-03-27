#pragma once

#include "ExportDll.h"

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
		PRICER_API double GetPrice(PnlMat* paths, double r,int t);
		PRICER_API double GetPayOff(PnlMat* paths) ;
		 
		PRICER_API double Dividendes(double r, int day);
		 
		PRICER_API PnlVectInt* GetDates();
		PRICER_API int GetMaturity();
		PRICER_API SousJacent* GetSousjacents();
		PRICER_API int GetSousjacentsSize();
		PRICER_API int GetNbForeignCurrency();
		PRICER_API int getDateIterateur(int date);
		PRICER_API Prisma();
		PRICER_API Prisma(PnlVectInt* _QuarterlyObservationDates, PnlVectInt* _AnnualObservationDates, PnlVectInt* _DividendesDates, PnlVectInt* _Dates);
		PRICER_API virtual ~Prisma();

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
		map<int, int>* InversedDates= new map<int,int>();
	};
}
