#include "stdafx.h"
#include "Data.h"


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
		SousJacent* SJ = _produit->GetSousjacents();
		for (int i = 0; i < datesSize; i++) {
			InversedSousJacents[(string)SJ[i].getName()] = i;
		}

		int tmp = produit->GetSousjacentsSize();
		VolatilityHistoricSousJacent = pnl_mat_create(datesSize, tmp);
		delthaHistoricSousJacent = pnl_mat_create(datesSize, tmp);
		PriceHistoricSousJacent = pnl_mat_create(datesSize, tmp);
		
		correlations = pnl_mat_create(tmp, tmp);

		tmp = produit->GetNbForeignCurrency();
		if (tmp > 0) {
			VolatilityHistoricCurency = pnl_mat_create(datesSize, tmp);
			delthaHistoricSousCurency = pnl_mat_create(datesSize, tmp);
			PriceHistoriCurency = pnl_mat_create(datesSize, tmp);
		}
		else
		{
			VolatilityHistoricCurency = NULL;
			delthaHistoricSousCurency = NULL;
			PriceHistoriCurency = NULL;
		}
		

		
	}
	int Data::FindIdSousJacent(SousJacent sj) {
		
		//return InversedSousJacents.find(sj.getName())->second;
		return InversedSousJacents[sj.getName()];
	}
	
	double Data::GetVolatilitySouJacent(SousJacent sj, int date)
	{
		/*pnl_mat_print(VolatilityHistoricSousJacent);
		int i = InversedDates[date];
		int j = FindIdSousJacent(sj);*/
		return pnl_mat_get(VolatilityHistoricSousJacent, InversedDates[date], FindIdSousJacent(sj));

	}
	void Data::GetAllVolatilitySouJacent(PnlVect * VolatilityVect, int date)
	{
		pnl_mat_get_row(VolatilityVect, VolatilityHistoricSousJacent, InversedDates[date]);
	}
	double Data::GetVolatilityCurency(SousJacent sj, int date)
	{
		if (VolatilityHistoricCurency == NULL) {
			throw new exception("no curency for this product");
		}
		return pnl_mat_get(VolatilityHistoricCurency, InversedDates[date], FindIdSousJacent(sj));
	}
	void Data::GetAllVolatilityCurency(PnlVect * VolatilityVect, int date)
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
	void Data::SetVectVolatilitySouJacent(const PnlVect * VolatilityVect, int date)
	{
		pnl_mat_set_row(VolatilityHistoricSousJacent, VolatilityVect, InversedDates[date]);
	}
}