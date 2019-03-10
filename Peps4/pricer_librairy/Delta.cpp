#include "stdafx.h"
#include "Delta.h"

namespace Pricer
{
	double epsilon = 0.01;
	static void GetDelta(PnlVect* Delta, Prisma produit, PnlMat* prices, int n, double r, int maturite, int t)
	{
		//double ecart = 0;
		//PnlMat* simul = new PnlMat(*prices);
		//PnlMat* simulPdt = new PnlMat(*prices);
		//PnlMat* simulMdt = new PnlMat(*prices);
		//for (int i = 0; i < n; i++) {
		//	simul ;// changement de simul 
		//	for (int j = 0; j < prices->n; j++) {
		//		simulPdt;// changement de simulPdt 
		//		simulMdt;// changement de simulMdt 
		//		ecart = produit.GetPayOff(simulPdt) - produit.GetPayOff(simulMdt);
		//		pnl_vect_set(Delta, i, GET(Delta, i) + ecart);
		//	}
		//}
		//PnlVect* tmp;
		//PnlVect* St;
		//pnl_mat_get_row(St, prices, t);
		//pnl_vect_clone(tmp, St);
		//pnl_vect_div_scalar(tmp, 2 * n*epsilon);
		//pnl_vect_mult_vect_term(Delta, tmp);

		//pnl_vect_free(&tmp);
		//pnl_mat_free(&simul);
		//pnl_mat_free(&simulPdt);
		//pnl_mat_free(&simulMdt);
	}
}
