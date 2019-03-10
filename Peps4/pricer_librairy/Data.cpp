#include "stdafx.h"
#include "Data.h"

using namespace std;

namespace Outils
{
	Data::Data(Produit* produit)
	{
		_produit = produit;
		int  datesSize = produit->GetDates()->size;
		PnlVectInt* dates = _produit->GetDates();
		for (int i = 0; i < datesSize; i++) {
			InversedDates[pnl_vect_int_get(dates, i)] = i;
		}
		int countCurrency = 0;
		int tmp = produit->GetSousjacentsSize();
		SousJacent* SJ = _produit->GetSousjacents();
		SousJacent sj;
		for (int i = 0; i < tmp; i++) {
			sj = SJ[i];
			InversedSousJacents[(string)sj.getName()] = i;

			if (InversedCurency.count(sj.monaie)==0&& sj.monaie!=EUR) {
				InversedCurency[sj.monaie] = countCurrency;
				countCurrency++;
			}
		}

		
		VolatilitySousJacent = pnl_vect_create_from_double(tmp,0.);
		VolatilityCurency = pnl_vect_create_from_double(tmp,0.);
		
		VolatilityHistoricSousJacent = pnl_mat_create_from_double(datesSize, tmp,0.);
		delthaHistoricSousJacent = pnl_mat_create_from_double(datesSize, tmp,0.);
		PriceHistoricSousJacent = pnl_mat_create_from_double(datesSize, tmp,0.);
		
		int tmp2 = produit->GetNbForeignCurrency();

		correlations = pnl_mat_create_from_double(tmp+ tmp2, tmp+ tmp2,1.);

		Rates = pnl_vect_create_from_double(1 + tmp2 , 0.);

		Hedge = pnl_vect_create_from_double(tmp + tmp2 + 1, 0.);
		HedgePriceHistory = pnl_vect_create_from_double(datesSize, 0.);
		ProductPriceHistory = pnl_vect_create_from_double(datesSize, 0.);
		SousJacentsPrice = pnl_mat_create_from_double(datesSize, tmp + tmp2,0.);

		if (tmp2 > 0) {
			VolatilityHistoricCurency = pnl_mat_create_from_double(datesSize, tmp2,0.);
			delthaHistoricCurency = pnl_mat_create_from_double(datesSize, tmp2,0.);
			PriceHistoriCurency = pnl_mat_create_from_double(datesSize, tmp2,0.);
		}
		else
		{
			VolatilityHistoricCurency = NULL;
			delthaHistoricCurency = NULL;
			PriceHistoriCurency = NULL;
		}
		

		
	}
	
	int Data::FindIdSousJacent(SousJacent sj) {
		
		//return InversedSousJacents.find(sj.getName())->second;
		return InversedSousJacents[sj.getName()];
	}
	

	double Data::GetHistoricVolatilitySouJacent(SousJacent sj, int date)
	{
		return pnl_mat_get(VolatilityHistoricSousJacent, InversedDates[date], FindIdSousJacent(sj));
	}
	void Data::GetHistoricAllVolatilitySouJacent(PnlVect * VolatilityVect, int date)
	{
		pnl_mat_get_row(VolatilityVect, VolatilityHistoricSousJacent, InversedDates[date]);
	}
	void Data::SetVectVolatilitySouJacent(const PnlVect * VolatilityVect, int date)
	{
		pnl_mat_set_row(VolatilityHistoricSousJacent, VolatilityVect, InversedDates[date]);
	}



	double Data::GetVolatilitySouJacent(SousJacent sj)
	{
		return pnl_vect_get(VolatilitySousJacent, FindIdSousJacent(sj));
	}
	void Data::SetVolatilitySouJacent(const PnlVect * vect)
	{
		pnl_vect_clone(VolatilitySousJacent, vect);
	}

	




	double Data::GetHistoricVolatilityCurency(SousJacent sj, int date)
	{
		if (VolatilityHistoricCurency == NULL) {
			throw new exception("no curency for this product");
		}
		return pnl_mat_get(VolatilityHistoricCurency, InversedDates[date], FindIdSousJacent(sj));
	}
	void Data::GetHistoricAllVolatilityCurency(PnlVect * VolatilityVect, int date)
	{
		if (VolatilityHistoricCurency == NULL) {
			throw new exception("no curency for this product");
		}
		pnl_mat_get_row(VolatilityVect, VolatilityHistoricCurency, InversedDates[date]);
	}
	void Data::SetVectVolatilityCurency(const PnlVect * VolatilityVect, int date)
	{
		pnl_mat_set_row(VolatilityHistoricCurency,VolatilityVect, InversedDates[date]);
	}



	
	double Data::GetVolatilityCurency(SousJacent sj)
	{
		return pnl_vect_get(VolatilityCurency, InversedCurency[sj.monaie]);
	}
	void Data::SetVolatilityCurency(const PnlVect * vect)
	{
		pnl_vect_clone(VolatilityCurency, vect);
	}





	void Data::GetPriceHistoricSousJacent(PnlMat * mat)
	{
		pnl_mat_set_subblock(mat,PriceHistoricSousJacent,0,0);
	}

	void Data::SetPriceHistoricSousJacent(const PnlVect * vect, int date)
	{
		pnl_mat_set_row(PriceHistoricSousJacent, vect, date);
	}

	void Data::GetPriceHistoricCurency(PnlMat * mat)
	{
		pnl_mat_set_subblock(mat, PriceHistoriCurency, 0, 0);
	}

	void Data::SetPriceHistoricCurency(const PnlVect * vect, int date)
	{
		pnl_mat_set_row(PriceHistoriCurency, vect, date);
	}
	




	void Data::GetDelthaHistoricSousJacent(PnlMat * deltas)
	{
		pnl_mat_set_subblock(deltas, delthaHistoricSousJacent, 0, 0);
	}

	void Data::SetDelthaHistoricSousJacent(const PnlVect * vect, int date)
	{
		pnl_mat_set_row(delthaHistoricSousJacent, vect, date);
	}


	PnlMat * Data::GetCorrelations()
	{
		return correlations;
	}
	double Data::GetCorrelations(SousJacent sj)
	{
		int i = FindIdSousJacent(sj);
		int j = _produit->GetSousjacentsSize() + InversedCurency[sj.monaie];
		return MGET(correlations,i,j);
	}
	void Data::SetCorrelations(PnlMat * correlation)
	{
		bool a = (int)correlation->m != (int)correlation->n ;
		bool b = ((int)correlation->m != (int)InversedSousJacents.size() + (int)InversedCurency.size());
		if (((int)correlation->m != (int)correlation->n )|| ((int)correlation->m != (int)InversedSousJacents.size() + (int)InversedCurency.size())) {
			printf( "wrong size of the corelation mat in  Data::SetCorrelations");
			throw new exception("wrong size of the corelation mat in  Data::SetCorrelations");
		}
		pnl_mat_set_subblock(correlations ,correlation,0,0);
	}
}