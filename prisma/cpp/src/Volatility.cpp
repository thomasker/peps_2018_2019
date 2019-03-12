
#include "Volatility.h"
namespace Pricer
{
	double Volatility::historicVolatility(PnlVect * historic, PnlVectInt *dates)
	{
		/*double toto = methode1(historic);
		double tata = methode2(historic);*/
		double tutu = methode3(historic, dates);
		return  tutu;
	}

	 void Volatility::historicVolatility(PnlVect * volatility, PnlMat * historic, PnlVectInt *dates)
	{
		 if (volatility->size != historic->n) {
			 pnl_vect_resize(volatility, historic->n);
		 }
		 PnlVect* tmp = pnl_vect_create(historic->m);
		 for (int i = 0; i < historic->n; i++) {
			 pnl_mat_get_col(tmp, historic, i);
			 pnl_vect_set(volatility, i, historicVolatility(tmp, dates));
		 }
		 pnl_vect_free(&tmp);
	}

	

	double Pricer::Volatility::methode1(PnlVect * historic) {
		double moyenne = 0;
		double variance = 0;
		for (int i = 0; i < historic->size; i++) {
			moyenne += GET(historic, i);
			variance += GET(historic, i)*GET(historic, i);
		}
		moyenne /= historic->size;
		variance = variance / historic->size - moyenne * moyenne;
		return  sqrt(variance);
	}

	double Pricer::Volatility::methode2(PnlVect * historic) {
		double moyenne = 0;
		for (int i = 0; i < historic->size; i++) {
			moyenne += GET(historic, i);
		}
		moyenne /= historic->size;
		double variance = 0;
		for (int i = 0; i < historic->size; i++) {
			variance += (GET(historic, i)- moyenne)*(GET(historic, i) - moyenne);
		}
		variance /= historic->size;
		return  sqrt(variance);
	}

	double Volatility::methode3(PnlVect *historic, PnlVectInt *dates) {
		double biai = 0;
		double moyenne = 0;
		double pas = 0;
		int n = historic->size;

		for (int i = 1; i < n; ++i) {
			pas = pnl_vect_int_get(dates, i) - pnl_vect_int_get(dates, i - 1);
			biai += pow(log(GET(historic, i) / GET(historic, i - 1)) / sqrt(pas), 2);
			moyenne += log(GET(historic, i) / GET(historic, i - 1)) / sqrt(pas);
		}

		return sqrt(biai / (n - 1) - pow(moyenne / (n - 1), 2));
	}

}