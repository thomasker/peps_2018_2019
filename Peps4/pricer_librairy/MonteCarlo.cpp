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
		//cout << "le prix est de :" << prix << "\n";
		double var = sumcarre / _n - prix * prix;
		intervalConfiance = 1.96*sqrt(var / _n);


		pnl_mat_free(&ST);
		pnl_mat_free(&Gmat);
		pnl_mat_free(&Gmat2);
		pnl_mat_free(&Correlations);
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
		prix = -1;
	}

	MonteCarlo::~MonteCarlo()
	{
	}

	double MonteCarlo::GetPrice()
	{
		if (prix <0) throw "need to run SimpleMonteCarlo before";
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
		//genere trop de nombre aleator e , quand il y a un passé 

		PnlVectInt* tvect = produit->GetDates();

		SousJacent* sousjacent = produit->GetSousjacents();
		int sousjacentSize = produit->GetSousjacentsSize();

		double shift = 1.0/100.0;// 1bp car pris en grandeur 100
		double ecart = 0;
		PnlMat* simul = pnl_mat_create(prices->m, prices->n);
		PnlMat* simulPdt = pnl_mat_create(prices->m, prices->n);
		PnlMat* simulMdt = pnl_mat_create(prices->m, prices->n);

		PnlMat* Gmat = pnl_mat_create(tvect->size, prices->m-1);//on retir la colone pour l euro 
		PnlMat* Gmat2 = pnl_mat_create(tvect->size, prices->m-1);
		//PnlMat* Correlations = pnl_mat_create(sousjacentSize, sousjacentSize);

		double payoff = 0.;
		double sum = 0.;
		double sumcarre = 0.;

		//pnl_mat_extract_subblock(Correlations, data.GetCorrelations(), 0, sousjacentSize, 0, sousjacentSize);

		PnlVect* shifter = pnl_vect_create(prices->n);
		PnlVect* zeroVect = pnl_vect_create(prices->n);
		for (int i =0; i < t + 1; i++) {
			pnl_vect_set(shifter, i, 0);
		}
		for (int i = t+1; i < prices->n; i++) {
			pnl_vect_set(shifter, i, shift);
		}

		PnlVect* rowToBeShifted = pnl_vect_create(tvect->size);
		for (int i = 0; i < n; i++) {

			//generation des mouvement browniens
			pnl_mat_rng_normal(Gmat, tvect->size, prices->m - 1, _rng);
			//pnl_mat_mult_mat_inplace(Gmat2, Gmat, Correlations);
			pnl_mat_mult_mat_inplace(Gmat2, Gmat, data.GetCorrelations());

			/*PnlMat* testmc2 = pnl_mat_create(prices->m, 10);
			pnl_mat_extract_subblock(testmc2, prices, 0, prices->m, 0, 10);
			cout << "prices :\n";
			pnl_mat_print(testmc2);*/
			//creation des trajectoires
			fillPaths(model, simul, prices, Gmat2, tvect, sousjacentSize, sousjacent, data, t);
			pnl_mat_clone(simulPdt, simul);
			pnl_mat_clone(simulMdt, simul);

			//calcul du payoff et de la presicion
			/*PnlMat* testmc1 = pnl_mat_create(simul->m, 10);
			pnl_mat_extract_subblock(testmc1, simul, 0, simul->m, 0, 10);
			cout << "simul :\n";
			pnl_mat_print(testmc1);*/

			payoff = produit->GetPrice(simul, _r, t);
			sum += payoff;
			sumcarre += payoff * payoff;

			for (int j = 0; j < prices->m; j++) {
				//reset the shifter mat
				if (j != 0) {
					pnl_mat_get_row(rowToBeShifted, simul, j - 1);
					pnl_mat_set_row(simulPdt, rowToBeShifted, j - 1);
					pnl_mat_set_row(simulMdt, rowToBeShifted, j - 1);
				}
				else {
					pnl_mat_get_row(rowToBeShifted, simul, prices->m - 1);
					pnl_mat_set_row(simulPdt, rowToBeShifted, prices->m - 1);
					pnl_mat_set_row(simulMdt, rowToBeShifted, prices->m - 1);
				}

				//shift the paths
				pnl_mat_get_row(rowToBeShifted, simul, j);
				pnl_vect_plus_vect(rowToBeShifted, shifter);
				pnl_mat_set_row(simulPdt,rowToBeShifted,j);

			
				pnl_mat_get_row(rowToBeShifted, simul, j);
				pnl_vect_minus_vect(rowToBeShifted, shifter);
				pnl_mat_set_row(simulMdt, rowToBeShifted, j);

				double tmp1 = produit->GetPrice(simulPdt, _r, t);
				double tmp2 = produit->GetPrice(simulMdt, _r, t);				
				
				ecart = tmp1 - tmp2;
				if (ecart >1) {// exeptional enents
					//ecart /= 100;
					cout << "c est pas normal pour j ="<< j<<" on a tmp1= " << tmp1 << " et tmp2 = " << tmp2 << "\n";
				}
				if (ecart < -1) {// exeptional enents
					//tata = true;
					cout << "c est pas normal pour j =" << j << " on a tmp1= " << tmp1 << " et tmp2 = " << tmp2 << "\n";
				}
				pnl_vect_set(Delta, j, GET(Delta, j) + ecart);
			}
		}

		prix = sum / n;
		//cout << "le prix est de :" << prix << "\n";
		double var = sumcarre / n - prix * prix;
		intervalConfiance = 1.96*sqrt(var / n);
		/*cout << "les delta avant operation : \n";
		pnl_vect_print(Delta);*/
		pnl_vect_div_scalar(Delta, 2 * n*shift);

		cout << "les delta  : \n";
		pnl_vect_print(Delta);


		pnl_vect_free(&shifter);
		pnl_vect_free(&zeroVect);
		pnl_vect_free(&rowToBeShifted);

		pnl_mat_free(&simulPdt);
		pnl_mat_free(&simulMdt);
		pnl_mat_free(&simul);
		pnl_mat_free(&Gmat);
		pnl_mat_free(&Gmat2);
		//pnl_mat_free(&Correlations);
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
			model->GetPriceEvolution2(tmp, G, tvect, _r, sigma, _S0, t + 1);
			//model->GetPriceEvolution2(tmp, G, tvect, _r, _thetha, _S0);
			pnl_mat_set_row(Paths, tmp, j);
		}
		pnl_vect_set_all(tmp, 1.);
		pnl_mat_set_row(Paths, tmp, sousjacentSize);
		SousJacent sj = SousJacent();
		for (int i = 1; i < Paths->m- sousjacentSize; i++)// boucle sur les sous jacents 
		{
			pnl_mat_get_col(G, Gmat, sousjacentSize+i-1);
			//GenerateRandVect(G,tvect->size);
			sj.monaie = (curency)i;
			sigma = data.GetVolatilityCurency(sj);
			if (past != NULL) {
				pnl_mat_get_row(tmp, past, sousjacentSize + i );
			}
			model->GetPriceEvolution2(tmp, G, tvect, 0, sigma, _S0, t + 1);
			//model->GetPriceEvolution2(tmp, G, tvect, _r, _thetha, _S0);
			pnl_mat_set_row(Paths, tmp, sousjacentSize + i);
		}
		pnl_vect_free(&G);
		pnl_vect_free(&tmp);

	}


	void MonteCarlo::GenerateNextDatePrices(BlackScholeModel * model, Produit* produit, Data data, int t) {
		PnlVect* G = pnl_vect_create(data.GetCorrelations()->m);
		PnlVect* tmp = pnl_vect_create(data.GetCorrelations()->m);
		double g = 0;
		double sigma = 0;
		double price = 0;
		pnl_vect_rng_normal(G, produit->GetSousjacentsSize() + produit->GetNbForeignCurrency(), _rng);
		
		for (int i = 0; i < produit->GetSousjacentsSize(); i++) {
			//pnl_vect_clone(tmp, G);// c est de la merde la non ? 
			pnl_mat_get_row(tmp, data.GetCorrelations(),i);
			pnl_vect_mult_vect_term(tmp, G);
			g = pnl_vect_sum(tmp);
			sigma = GetRightSigma(produit->GetSousjacents()[i], data);
			price = model->GetNextPrice2(g, produit->GetDates(), t, _r, sigma, MGET(data.SousJacentsPrice, t - 1,i));
			pnl_mat_set(data.SousJacentsPrice,t, i, price);
		}
		pnl_mat_set(data.SousJacentsPrice, t, produit->GetSousjacentsSize() ,1);// le taux eur/eur
		SousJacent sj = SousJacent();
		for (int i = 1; i < produit->GetNbForeignCurrency()+1; i++) {
			pnl_mat_get_row(tmp, data.GetCorrelations(), produit->GetSousjacentsSize()-1 +i);
			pnl_vect_mult_vect_term(tmp, G);
			g = pnl_vect_sum(tmp);
			sj.monaie = (curency)i;
			sigma = data.GetVolatilityCurency(sj);
			//cout << "g :" << g << " t :" << t << " r :" << 0 << "  sigma :" << sigma <<" St :"<< MGET(data.SousJacentsPrice, t - 1, produit->GetSousjacentsSize() + i) << "\n";
			price = model->GetNextPrice2(g, produit->GetDates(), t, 0, sigma, MGET(data.SousJacentsPrice, t - 1, produit->GetSousjacentsSize() + i));
			//cout << "price :" << price << "\n";
			pnl_mat_set(data.SousJacentsPrice, t, produit->GetSousjacentsSize()+i, price);
			//pnl_mat_set(data.CurencyRates, t, i, price);
		}
		pnl_vect_free(&G);
		pnl_vect_free(&tmp);
	}

}