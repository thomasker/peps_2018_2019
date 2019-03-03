#include "stdafx.h"
#include "MonteCarlo.h"
#include "pnl/pnl_root.h" 

#include <time.h>
using namespace std;

namespace Methodes
{
	double MonteCarlo::GetRightSigma(SousJacent sj, Data data)
	{
		if (sj.monaie == EUR) {

			return data.GetVolatilitySouJacent(sj);
		}
		else {
			double volsj = data.GetVolatilitySouJacent(sj);
			double volcur = data.GetVolatilityCurency(sj);
			double corelation = data.GetCorrelations(sj);
			return sqrt(volsj*volsj + volcur * volcur + 2 * volsj* volcur*corelation);
		}
	}
	void MonteCarlo::GenerateRandVect(PnlVect* G, int n, PnlRng *rng) {
		if (rng != NULL) {
			_rng = pnl_rng_copy(rng);
		}
		pnl_vect_rng_normal(G, n, _rng);
	}
	void MonteCarlo::GenerateMBSVect(PnlVect* W, int n, PnlRng *rng) {
		GenerateRandVect(W, n, rng);
		for (int i = 1; i < W->size; i++) {
			pnl_vect_set(W, i, GET(W, i - 1) + GET(W, i));
		}
	}

	void MonteCarlo::SimpleMonteCarlo(BlackScholeModel * model, Produit * produit, Data data)
	{
		MonteCarloWithPast(model, produit, data);
	}

	void MonteCarlo::MonteCarloWithPast(BlackScholeModel * model, Produit * produit, Data data, PnlMat * past, int t)
	{
		//genere trop de nombre aleator e , quand il y a un passé 
		double sum = 0;
		double sumcarre = 0;
		double payoff = 0;
		double sigma = 0;

		PnlVectInt* tvect = produit->GetDates();
		SousJacent* sousjacent = produit->GetSousjacents();
		int sousjacentSize = produit->GetSousjacentsSize();
		PnlMat* ST = pnl_mat_create(sousjacentSize, tvect->size);
		PnlMat* Gmat = pnl_mat_create(tvect->size, sousjacentSize);// nb ligne = nb de dates 
		PnlMat* Gmat2 = pnl_mat_create(tvect->size, sousjacentSize);
		PnlMat* Correlations = pnl_mat_create(sousjacentSize, sousjacentSize);

		pnl_mat_extract_subblock(Correlations, data.GetCorrelations(), 0, sousjacentSize, 0, sousjacentSize);
		for (int i = 0; i < _n; i++) // boucle sur les iteration de N 
		{
			pnl_mat_rng_normal(Gmat, tvect->size, sousjacentSize, _rng);
			pnl_mat_mult_mat_inplace(Gmat2, Gmat, Correlations);


			fillPaths(model, ST, past, Gmat2, tvect, sousjacentSize, sousjacent, data, t);

			// calcul du prix 
			payoff = produit->GetPrice(ST, _r, t);
			sum += payoff;
			sumcarre += payoff * payoff;
		}
		prix = sum / _n;
		cout << "le prix est de :" << prix << "\n";
		double var = sumcarre / _n - prix * prix;
		intervalConfiance = 1.96*sqrt(var / _n);


		pnl_mat_free(&ST);
		pnl_mat_free(&Gmat);
		pnl_mat_free(&Gmat2);
		pnl_mat_free(&Correlations);
	}

	void MonteCarlo::fillPaths(BlackScholeModel * model, PnlMat* Paths, PnlMat * past, PnlMat* Gmat, PnlVectInt* tvect, int sousjacentSize, SousJacent* sousjacent, Data data, int t) {
		PnlVect* G = pnl_vect_create(Gmat->m);
		PnlVect* tmp = pnl_vect_create_from_double(Gmat->m, 0.);
		double sigma = 0;
		for (int j = 0; j < sousjacentSize; j++)// boucle sur les sous jacents 
		{
			pnl_mat_get_col(G, Gmat, j);
			//GenerateRandVect(G,tvect->size);
			sigma = GetRightSigma(sousjacent[j], data);
			if (past != NULL) {
				pnl_mat_get_row(tmp, past, j);
			}
			model->GetPriceEvolution2(tmp, G, tvect, _r, sigma, _S0, t);
			//model->GetPriceEvolution2(tmp, G, tvect, _r, _thetha, _S0);
			pnl_mat_set_row(Paths, tmp, j);
		}
		pnl_vect_free(&G);
		pnl_vect_free(&tmp);

	}

	MonteCarlo::MonteCarlo(int n, double r, int t, double thetha, double S0)
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

	void MonteCarlo::GetDelta(PnlVect* Delta, BlackScholeModel * model, Produit* produit, Data data, PnlMat* prices, int n, int t)
	{//confusion dimmention n et m a corrigé 
		double test = 0;
		PnlVectInt* tvect = produit->GetDates();

		SousJacent* sousjacent = produit->GetSousjacents();
		int sousjacentSize = produit->GetSousjacentsSize();

		double shift = 1.0/100.0;// 1bp car pris en grandeur 100
		double ecart = 0;
		PnlMat* simul = pnl_mat_create(prices->m, prices->n);
		PnlMat* simulPdt = pnl_mat_create(prices->m, prices->n);
		PnlMat* simulMdt = pnl_mat_create(prices->m, prices->n);

		PnlMat* Gmat = pnl_mat_create(tvect->size, sousjacentSize);
		PnlMat* Gmat2 = pnl_mat_create(tvect->size, sousjacentSize);
		PnlMat* Correlations = pnl_mat_create(sousjacentSize, sousjacentSize);

		pnl_mat_extract_subblock(Correlations, data.GetCorrelations(), 0, sousjacentSize, 0, sousjacentSize);

		PnlMat * matShifter = pnl_mat_create(prices->m, prices->n);
		PnlVect* shifter = pnl_vect_create(prices->n);
		PnlVect* zeroVect = pnl_vect_create(prices->n);
		for (int i = t; i < prices->n; i++) {
			pnl_vect_set(shifter, i, shift);
		}
		PnlVect* tmp3 = pnl_vect_create(tvect->size);
		PnlVect* rowToBeShifted = pnl_vect_create(tvect->size);
		for (int i = 0; i < n; i++) {

			pnl_vect_rng_normal(tmp3, tvect->size, _rng);
			for (int k = 0; k < prices->m; k++) {
				pnl_mat_set_col(Gmat2, tmp3, k);
			}
			//pnl_mat_rng_normal(Gmat, tvect->size, sousjacentSize, _rng);
			///*cout << "Matrice random : " << "\n";
			//pnl_mat_print(Gmat);*/
			///*cout << "correlation : " << "\n";
			//pnl_mat_print(Correlations);*/
			//pnl_mat_mult_mat_inplace(Gmat2, Gmat, Correlations);
			///*cout << "Matrice random corélé  : " << "\n";*/
			//pnl_mat_print(Gmat2);
			fillPaths(model, simul, prices, Gmat2, tvect, sousjacentSize, sousjacent, data, t);// retiré  aleatoire 
			//pnl_mat_print(simul);
			pnl_mat_clone(simulPdt, simul);
			pnl_mat_clone(simulMdt, simul);
			for (int j = 0; j < prices->m; j++) {
				//reset the shifter mat
				if (j != 0) {
					pnl_mat_get_row(rowToBeShifted, simul, j - 1);
					pnl_mat_set_row(simulPdt, rowToBeShifted, j - 1);
					pnl_mat_set_row(simulMdt, rowToBeShifted, j - 1);
					//pnl_mat_set_row(matShifter, zeroVect ,j-1);
				}
				else {
					pnl_mat_get_row(rowToBeShifted, simul, prices->m - 1);
					pnl_mat_set_row(simulPdt, rowToBeShifted, prices->m - 1);
					pnl_mat_set_row(simulMdt, rowToBeShifted, prices->m - 1);
					//pnl_mat_set_row(matShifter, zeroVect, prices->m-1);
				}

				pnl_mat_get_row(rowToBeShifted, simul, j);
				pnl_vect_plus_vect(rowToBeShifted, shifter);
				pnl_mat_set_row(simulPdt,rowToBeShifted,j);

				pnl_mat_get_row(rowToBeShifted, simul, j);
				pnl_vect_minus_vect(rowToBeShifted, shifter);
				pnl_mat_set_row(simulMdt, rowToBeShifted, j);


				//pnl_mat_set_row(matShifter, shifter, j);
				//set simul + ds
				/*pnl_mat_set_all(simulPdt, 0);
				pnl_mat_plus_mat(simulPdt, matShifter);
				pnl_mat_plus_mat(simulPdt, simul);*/
				/*cout << "simulPdt +++++ avec j = " << j << " : " << "\n";
				pnl_mat_print(simulPdt);*/
				//set simul - ds
				/*pnl_mat_set_all(simulMdt, 0);
				pnl_mat_minus_mat(simulMdt, matShifter);
				pnl_mat_plus_mat(simulMdt, simul);*/
				/*cout << "simulMdt ----- avec j = "<< j <<" : " << "\n";
				pnl_mat_print(simulMdt);*/

				/*pnl_mat_get_row(tmp3, simulPdt, j);
				pnl_vect_print(tmp3); 

				pnl_mat_get_row(tmp3, simulMdt, j);
				pnl_vect_print(tmp3);*/

				double tmp1 = produit->GetPayOff(simulPdt);
				double tmp2 = produit->GetPayOff(simulMdt);
				test += tmp1;
				//ecart = produit->GetPayOff(simulPdt) - produit->GetPayOff(simulMdt);
				ecart = tmp1 - tmp2;
				pnl_vect_set(Delta, j, GET(Delta, j) + ecart);
			}
		}
		this->prix = test / n;
		pnl_vect_print(Delta);
		PnlVect* tmp = pnl_vect_create(prices->m);
		PnlVect* St = pnl_vect_create(prices->m);
		pnl_mat_get_col(St, prices, t);
		pnl_vect_print(St);
		pnl_vect_clone(tmp, St);
		pnl_vect_print(tmp);
		pnl_vect_div_scalar(tmp, 2 * n*shift);
		pnl_vect_print(tmp);
		pnl_vect_mult_vect_term(Delta, tmp);

		pnl_vect_free(&tmp);
		pnl_vect_free(&shifter);
		pnl_vect_free(&zeroVect);
		pnl_mat_free(&simulPdt);
		pnl_mat_free(&simul);
		
		pnl_mat_free(&simulMdt);
		pnl_mat_free(&Gmat);
		pnl_mat_free(&Gmat2);
		pnl_mat_free(&Correlations);
		pnl_mat_free(&matShifter);
	}

	void MonteCarlo::GenerateNextDatePrices(PnlVect* prices, PnlVect* pastPrices, BlackScholeModel * model, Produit* produit,int t, Data data) {
		PnlVect* G = pnl_vect_create(produit->GetSousjacentsSize());
		PnlVect* tmp = pnl_vect_create(produit->GetSousjacentsSize());
		PnlMat* correlation = pnl_mat_create(produit->GetSousjacentsSize(), produit->GetSousjacentsSize());
		double g = 0;
		double sigma = 0;
		double price = 0;
		pnl_vect_rng_normal(G, produit->GetSousjacentsSize(), _rng);
		
		pnl_mat_extract_subblock(correlation, data.GetCorrelations(), 0, produit->GetSousjacentsSize(), 0, produit->GetSousjacentsSize());
		for (int i = 0; i < produit->GetSousjacentsSize(); i++) {
			pnl_vect_clone(tmp, G);
			pnl_mat_get_row(tmp, data.GetCorrelations(),i);
			pnl_vect_mult_vect_term(tmp, G);
			g = pnl_vect_sum(tmp);
			sigma = GetRightSigma(produit->GetSousjacents()[i], data);
			price = model->GetNextPrice2(g, produit->GetDates(), t, _r, sigma, GET(pastPrices, i));
			pnl_vect_set(prices, i, price);
		}

	}
}