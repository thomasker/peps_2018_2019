#pragma once
#include "ExportDll.h"

#include "pnl/pnl_matrix.h"
#include <map>
//#include <iostream>
//#include <cassert>
#include <exception>

#include "Produit.h"
#include "sousjacent.h"




using namespace std;
using namespace Produits;

namespace Outils
{
	class Data {
	private:
		Produit* _produit;
		map<int, int> InversedDates;
		map<string, int> InversedSousJacents;

		PnlMat* VolatilityHistoricSousJacent;
		PnlMat* VolatilityHistoricCurency;
		//hash map sous jacent to numereau dans la liste 
		PnlMat* delthaHistoricSousJacent;
		PnlMat* delthaHistoricSousCurency;
		PnlMat* PriceHistoricSousJacent;
		PnlMat* PriceHistoriCurency;


		PnlMat* correlations;

		int FindIdSousJacent(SousJacent date);

	public:
		Data(Produit* produit);
		double GetVolatilitySouJacent(SousJacent sj, int date);
		void GetAllVolatilitySouJacent(PnlVect* VolatilityVect, int date);
		double GetVolatilityCurency(SousJacent sj, int date);
		void GetAllVolatilityCurency(PnlVect* VolatilityVect, int date);

		void SetVectVolatilityCurency(const PnlVect* VolatilityVect, int date);
		void SetVectVolatilitySouJacent(const PnlVect* VolatilityVect, int date);
	};
}