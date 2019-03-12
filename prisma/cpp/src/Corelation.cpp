#include "Corelation.h"
#include <cmath>

namespace Pricer
{
	double Corelation::GetCorelation(PnlVect * productA, PnlVect * productB)
	{
		//initialisation 
		int size = productA->size - productB->size < 0 ? productB->size - 1 : productA->size - 1;   ;// care there is a -1 here 
		PnlVect *log_renta_A = pnl_vect_create(size);
		PnlVect *log_renta_B = pnl_vect_create(size);
		double moyenne_A = 0;
		double moyenne_B = 0;

		for (int i = 0; i < size; ++i) {
			LET(log_renta_A, i) = log(GET(productA, i + 1) / GET(productA, i));
			LET(log_renta_B, i) = log(GET(productB, i + 1) / GET(productB, i));
			moyenne_A += GET(log_renta_A, i);
			moyenne_B += GET(log_renta_B, i);
		}
		moyenne_A /= size;
		moyenne_B /= size;
		//Calcul de correlation 
		double covariance = 0;
		double var_A = 0;
		double var_B = 0;

		for (int i = 0; i < size; ++i) {
			covariance += (GET(log_renta_A, i) - moyenne_A) * (GET(log_renta_B, i) - moyenne_B);
			var_A += pow(GET(log_renta_A, i) - moyenne_A, 2);
			var_B += pow(GET(log_renta_B, i) - moyenne_B, 2);
		}

		covariance /= size * size;
		var_A /= size * size;
		var_B /= size * size;

		// Free memory
		pnl_vect_free(&log_renta_A);
		pnl_vect_free(&log_renta_B);

		if (var_A <= 0 || var_B <= 0) {
			return 0.;
		}
		return covariance / sqrt(var_A * var_B);
	}
	void Corelation::GetMatCorelation(PnlMat * correlations, PnlMat * products)
	{
		// verifier que correlation est bien de taille products->n 2
		pnl_mat_resize(correlations, products->n, products->n);
		PnlVect * productA = pnl_vect_create(products->m);
		PnlVect * productB = pnl_vect_create(products->m);
		double tmp = 0;
		for (int i = 0; i < products->n; i++) {
			for (int j = 0; j <= i; j++) {
				tmp = GetCorelation(productA, productB);
				pnl_mat_set(correlations, i, j, tmp);
				pnl_mat_set(correlations, j, i, tmp);
			}
		}
		pnl_mat_chol(correlations);
	}
}