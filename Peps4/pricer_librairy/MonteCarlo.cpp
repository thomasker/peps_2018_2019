#include "stdafx.h"
#include "MonteCarlo.h"
#include "pnl/pnl_root.h" 

#include <time.h>
using namespace std;

namespace Methodes
{
	double MonteCarlo::GetRightSigma(SousJacent sj, Data data, int date )
	{
		if (sj.monaie == EUR) {

			return data.GetVolatilitySouJacent(sj, date);
		}
		else {
			throw new exception("not implemented yet exeption");
		}
	}
	void MonteCarlo::GenerateRandVect(PnlVect* G, int n, PnlRng *rng) {
		if (rng != NULL) {
			_rng = pnl_rng_copy(rng) ;
		}
		pnl_vect_rng_normal(G, n, _rng);
	}
	void MonteCarlo::GenerateMBSVect(PnlVect* W,int n, PnlRng *rng) {
		GenerateRandVect(W,n, rng);
		for (int i = 1; i < W->size; i++) {
			pnl_vect_set(W, i, GET(W, i - 1) + GET(W, i));
		}
	}

	void MonteCarlo::SimpleMonteCarlo(BlackScholeModel * model, Produit * produit, Data data)
	{
		double sum = 0;
		double sumcarre = 0;
		double payoff = 0;
		double sigma = 0;
		PnlVect* tmp = pnl_vect_create(1);
		PnlVectInt* tvect = produit->GetDates();
		SousJacent* sousjacent = produit->GetSousjacents();
		int sousjacentSize = produit->GetSousjacentsSize();
		PnlVect* G = pnl_vect_create(tvect->size);
		PnlMat* ST = pnl_mat_create(sousjacentSize, tvect->size);
		for (int i = 0; i < _n; i++) // boucle sur les iteration de N 
		{
			for (int j = 0; j < sousjacentSize; j++)// boucle sur les sous jacents 
			{
				GenerateRandVect(G,tvect->size);
				sigma = GetRightSigma(sousjacent[j], data,0);
				model->GetPriceEvolution2(tmp, G, tvect, _r, sigma, _S0);
				//model->GetPriceEvolution2(tmp, G, tvect, _r, _thetha, _S0);
				pnl_mat_set_row(ST, tmp, j);
			}
			// calcul du prix 
			payoff = produit->GetPrice(ST,_r,0);
			sum += payoff;
			sumcarre += payoff * payoff;
		}
		prix = sum / _n;
		cout << "le prix est de :" << prix << "\n";
		double var = sumcarre / _n - prix * prix;
		intervalConfiance = 1.96*sqrt(var / _n);
	}
	MonteCarlo::MonteCarlo(int n, double r, int t,double thetha, double S0)
	{
		_rng = pnl_rng_create(PNL_RNG_MERSENNE);
		pnl_rng_sseed(_rng, (unsigned long)time(NULL));
		_n = n;
		_r = r;
		_T = t;
		_thetha = thetha;
		_S0 = S0;
	}

	MonteCarlo::~MonteCarlo()
	{
	}

	double MonteCarlo::GetPrice()
	{
		if (prix == NULL) throw "need to run SimpleMonteCarlo before";
		return prix;
	}

	double MonteCarlo::GetIntervalConfiance()
	{
		if (intervalConfiance == NULL) throw "need to run SimpleMonteCarlo before";
		return intervalConfiance;
	}

	double epsilon = 0.01;

	void MonteCarlo::GetDelta(PnlVect* Delta, Produit* produit, PnlMat* prices, int n, double r, int maturite, int t)
	{
		double ecart = 0;
		PnlMat* simul = new PnlMat(*prices);
		PnlMat* simulPdt = new PnlMat(*prices);
		PnlMat* simulMdt = new PnlMat(*prices);
		for (int i = 0; i < n; i++) {
			simul;// changement de simul 
			for (int j = 0; j < prices->n; j++) {
				simulPdt;// changement de simulPdt 
				simulMdt;// changement de simulMdt 
				ecart = produit->GetPayOff(simulPdt) - produit->GetPayOff(simulMdt);
				pnl_vect_set(Delta, i, GET(Delta, i) + ecart);
			}
		}
		PnlVect* tmp=pnl_vect_create(prices->m);
		PnlVect* St= pnl_vect_create(prices->m);
		pnl_mat_get_row(St, prices, t);
		pnl_vect_clone(tmp, St);
		pnl_vect_div_scalar(tmp, 2 * n*epsilon);
		pnl_vect_mult_vect_term(Delta, tmp);

		pnl_vect_free(&tmp);
		pnl_mat_free(&simul);
		pnl_mat_free(&simulPdt);
		pnl_mat_free(&simulMdt);
	}

}