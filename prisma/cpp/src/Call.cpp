
#include "Call.h"
#include <math.h>


namespace Produits
{
	double Call::GetPrice(PnlMat * paths, double r, int t)
	{
		return  GetPayOff(paths)*exp(-r * (_T - t)) ;
	}
	double Call::GetPayOff(PnlMat* paths)
	{
		double St = MGET(paths,0, paths->n - 1);
		return St < _K ? 0 : St - _K;
	}
	PnlVectInt * Call::GetDates()
	{
		return dates;
	}
	SousJacent* Call::GetSousjacents()
	{
		return sousJacents;
	}
	int Call::GetSousjacentsSize()
	{
		return sousJacentsSize;
	}
	int Call::GetNbForeignCurrency()
	{
		return 0;
	}
	Call::Call(double K, date start, date end){
		_K = K;
		dates = pnl_vect_int_create(1);
		TimeManager::fillOpenDates(dates,start,end);
		_T =(int) pnl_vect_int_get(dates,dates->size-1);
		SousJacent sousjacent;
		char name[] = "SousJacent";
		sousjacent.monaie = EUR;
		sousjacent.setName(name);
		sousJacents[0] = sousjacent;
	}
	Call::~Call(){}
}