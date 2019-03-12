
#include "GetDataSimul.h"


#include <time.h>
using namespace std;

namespace Outils
{
	void GetDataSimul::LoadData(int nday, Data data)
	{
		double price;
		double monaie;
		double g;
		double sigma;
		
		int sousjacentSize = data._produit->GetSousjacentsSize();
		int nbCurencys = data._produit->GetNbForeignCurrency();

		SousJacent sj;
		SousJacent* sousjacents = data._produit->GetSousjacents();

		PnlVectInt* tvect = data._produit->GetDates();

		PnlMat* oldprices = pnl_mat_create(tvect->size, sousjacentSize);
		PnlMat* oldcurencys = pnl_mat_create(tvect->size, nbCurencys);
		PnlMat* Correlations = data.GetCorrelations();

		PnlVect* ST = pnl_vect_create(sousjacentSize);
		PnlVect* newtaux = pnl_vect_create(nbCurencys);
		PnlVect* Gvect = pnl_vect_create(Correlations->m);

		curency monaiAray[100];//error if more that 100  curency , cant hapen i hope

		data.GetPriceHistoricSousJacent(oldprices);
		data.GetPriceHistoricSousJacent(oldcurencys);

		//generateur aleatoir 
		PnlRng*_rng = pnl_rng_create(PNL_RNG_MERSENNE);
		pnl_rng_sseed(_rng, (unsigned long)time(NULL));
		pnl_vect_rng_normal(Gvect, data._produit->GetSousjacentsSize(), _rng);


		for (int j = 0; j < sousjacentSize; j++)// boucle sur les sous jacents 
		{
			sj = sousjacents[j];
			PnlVect viewCorr = pnl_vect_wrap_mat_row(Correlations, j);
			g = pnl_vect_scalar_prod(&viewCorr, Gvect);
			//Get the right sigma
			 if (sj.monaie == EUR) {

				 sigma= data.GetVolatilitySouJacent(sj);
			}
			else {
				double volsj = data.GetVolatilitySouJacent(sj);
				double volcur = data.GetVolatilityCurency(sj);
				double corelation = data.GetCorrelations(sj);
				sigma = sqrt(volsj*volsj + volcur * volcur + 2 * volsj* volcur*corelation);
			}
			
			price=data._model->GetNextPrice2(g, tvect, nday, _r, sigma, MGET(oldprices, nday, j));
			pnl_vect_set(ST, j, price);
		}
		data.SetPriceHistoricSousJacent(ST, nday);

		
		sj = SousJacent();
		
		for (int j = 0; j < nbCurencys; j++)// boucle sur les curency
		{
			sj.monaie = monaiAray[j];
			PnlVect viewCorr = pnl_vect_wrap_mat_row(Correlations, j);
			g = pnl_vect_scalar_prod(&viewCorr, Gvect);
			monaie = MGET(oldprices, nday, j)+ g* data.GetVolatilityCurency(sj);;
			pnl_vect_set(newtaux, j, monaie);
		}
		data.SetPriceHistoricCurency(newtaux, nday);

	}
	void GetDataSimul::LoadAllData(int nstart, int nend, Data data)
	{
		for (int i = nstart; i < nend; i++) {
			LoadData(i, data);
		}
	}
}