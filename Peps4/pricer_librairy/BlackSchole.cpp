#include "stdafx.h" 
#include "BlackSchole.h"
#include "pnl/pnl_root.h" 

namespace Models
{

	double BlackScholeModel::GetPriceMatutity(PnlVect* W, double t, double r, double theta, double S0)
	{
		if (W == NULL) throw "W is Null";

		return S0 * exp((r - theta * theta / 2)*t + theta * GET(W, W->size - 1));
	}

	void BlackScholeModel::GetPriceEvolution1(PnlVect* paths,PnlVect * W, PnlVectInt * tvect, double r, double thetha, double S0, int t0 )
	{
		if (W == NULL || tvect == NULL) throw "W or Tvect are Null";
		if (W->size != tvect->size) throw "W and Tvect are of different size";

		pnl_vect_resize(paths,tvect->size);
		if (t0 <= 0) {
			pnl_vect_set(paths, 0, S0);
			t0 = 1;
		}
		for (int i = t0; i < tvect->size; i++) {
			pnl_vect_set(paths, i, GetNextPrice1(W, tvect, i, r, thetha, GET(paths, i - 1)));
		}
	}

	void BlackScholeModel::GetPriceEvolution2(PnlVect* paths, PnlVect * G, PnlVectInt * tvect, double r, double thetha, double S0, int t0)
	{
		if (G == NULL || tvect == NULL) throw "G or Tvect are Null";
		if (G->size != tvect->size ) throw "G and Tvect are of different size";

		pnl_vect_resize(paths, tvect->size);
		if (t0 <= 0) {
			pnl_vect_set(paths, 0, S0);
			t0 = 1;
		}
		for (int i = t0; i < tvect->size; i++) {
			pnl_vect_set(paths, i, GetNextPrice2(GET(G, i - 1), tvect, i, r, thetha, GET(paths, i - 1)));
		}
	}

	double BlackScholeModel::GetNextPrice1(PnlVect * W, PnlVectInt * tvect, int nt, double r, double thetha, double St)
	{
		if (W == NULL || tvect == NULL) throw "W or Tvect are Null";
		if (W->size != tvect->size) throw "W and Tvect are of different size";
		if (tvect->size < nt) throw "nt is out of range";
		//st*exp( (r-0**2)*(tn+1-tn) + 0*(wn+1-wn)) 
		return St * exp((r - thetha * thetha / 2)*(pnl_vect_int_get(tvect, nt) - pnl_vect_int_get(tvect, nt - 1)) + thetha * (GET(W, nt) - GET(W, nt - 1)));
	}

	double BlackScholeModel::GetNextPrice2(double G, PnlVectInt * tvect, int nt, double r, double thetha, double St)
	{
		if (tvect == NULL) throw "W or Tvect are Null";
		if (tvect->size < nt) throw "nt is out of range";

		//st*exp( (r-0**2/2)*(tn+1-tn) + 0*sqrt(tn+1-tn) )
		double timeSpace = pnl_vect_int_get(tvect, nt) - pnl_vect_int_get(tvect, nt - 1);
		return St * exp((r - thetha * thetha / 2)*timeSpace + thetha * sqrt(timeSpace)*G);
	}

	 void BlackScholeModel::GetMatPrices(PnlMat * paths, PnlMat * G, PnlVectInt * tvect, double r, double thetha, double S0, int t0)
	{
		 //verif le paths->m == tvect size

		 PnlVect* path = pnl_vect_create(paths->m);
		 PnlVect* tmp = pnl_vect_create(G->m);
		 for (int i = 0; i < paths->n; i++) {
			 pnl_mat_get_col(path,paths,i);
			 pnl_mat_get_col(tmp, G, i);
			 GetPriceEvolution2(path, tmp, tvect, r, thetha, S0, t0);
			 pnl_mat_set_col(paths, path, i);
		 }
		 pnl_vect_free(&path);
		 pnl_vect_free(&tmp);
	}
}