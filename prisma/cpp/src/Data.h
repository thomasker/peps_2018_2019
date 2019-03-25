#pragma once
#include "pnl/pnl_matrix.h"
#include <map>
#include <exception>

#include "Produit.h"
#include "BlackSchole.h"
#include "sousjacent.h"




using namespace std;
using namespace Produits;
using namespace Models;

namespace Outils
{
	class Data {
	private:
		
		map<int, int> InversedDates;
		map<string, int> InversedSousJacents;
		map<curency, int> InversedCurency;

		PnlMat* VolatilityHistoricSousJacent;
		PnlVect* VolatilitySousJacent;

		PnlMat* VolatilityHistoricCurency;
		PnlVect* VolatilityCurency;

		PnlMat* delthaHistoricSousJacent;
		PnlMat* delthaHistoricCurency;

		PnlMat* PriceHistoricSousJacent;
		PnlMat* PriceHistoriCurency;

		

		PnlMat* correlations;

		int FindIdSousJacent(SousJacent sj);

	public:
		Produit* _produit;
		BlackScholeModel* _model;
		Data(Produit* produit);

		PnlVect* ZeroRates; // taux de rendement (eur -- autres curency) 
		PnlVect* CurencyRates; // taux de change ( 1 -- autre taux de change)
		PnlVect* Hedge; // constitution du portefuille de couverture a une date ( sous jecents  -- zero coupons )
		PnlVect* HedgePriceHistory;// historique des prix du portefeuille de couverture 
		PnlVect* ProductPriceHistory; // historique des prix du produit

		PnlMat* SousJacentsPrice; // historique des prix des sous jacents avec une ligne par date et chaque ligne:( sous jacents  -- Curency)

		double GetHistoricVolatilitySouJacent(SousJacent sj, int date);
		void GetHistoricAllVolatilitySouJacent(PnlVect* VolatilityVect, int date);
		void SetVectVolatilitySouJacent(const PnlVect* VolatilityVect, int date);

		double GetVolatilitySouJacent(SousJacent sj);
		void SetVolatilitySouJacent(const PnlVect* vect );

		double GetHistoricVolatilityCurency(SousJacent sj, int date);
		void GetHistoricAllVolatilityCurency(PnlVect* VolatilityVect, int date);
		void SetVectVolatilityCurency(const PnlVect* VolatilityVect, int date);

		double GetVolatilityCurency(SousJacent sj);
		void SetVolatilityCurency(const PnlVect* vect);
		

		void GetPriceHistoricSousJacent(PnlMat* mat);
		void SetPriceHistoricSousJacent(const PnlVect* vect, int date);

		void GetPriceHistoricCurency(PnlMat* mat);
		void SetPriceHistoricCurency(const PnlVect* vect, int date);


		void GetDelthaHistoricSousJacent(PnlMat* deltas); 
		void SetDelthaHistoricSousJacent(const PnlVect* vect, int date);

		PnlMat* GetCorrelations();
		double GetCorrelations(SousJacent sj);
		void SetCorrelations(PnlMat* correlation);
	};
}