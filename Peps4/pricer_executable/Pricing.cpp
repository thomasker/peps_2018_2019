#include "Pricing.h"


namespace Pricing
{
	void PricingOptions::pricingCall()
	{
		double K = 110;
		double S0 = 100;
		date start = date(2, 1, 2002);
		date end = date(6, 1, 2002);
		PnlVectInt* dates = pnl_vect_int_create(1);
		TimeManager::fillOpenDates(dates, start, end);
		int T = pnl_vect_int_get(dates,dates->size -1);
		int n = 50000;
		double r = 0.1;
		double thetha = 0.01;

		Call* option = new Call(K, start,date(6,1,2002));
		BlackScholeModel* BnS = new BlackScholeModel();
		MonteCarlo  mc = MonteCarlo(n,r,T,thetha,S0);
		Data data = Data(option);
		mc.SimpleMonteCarlo(BnS,option,data);
		double Price = mc.GetPrice();
		double IC = mc.GetIntervalConfiance();
		
		double* price = new double(); 
		*price = 0;
		double *deltha = new double();
		*deltha = 0;
		double tot = pnl_cf_call_bs(S0, K, T, r, 0, thetha, price, deltha);
	}
	void PricingOptions::pricingPrisma()
	{
		double S0 = 100;
		int n = 50000;
		double r = 0.1/365;
		double thetha = 0.01;

		Prisma* option = new Prisma();
		int T = option->GetMaturity();
		BlackScholeModel* BnS = new BlackScholeModel();
		MonteCarlo  mc = MonteCarlo(n, r, T, thetha, S0);
		Data data = Data(option);
		mc.SimpleMonteCarlo(BnS, option, data);
		double Price = mc.GetPrice();
		double IC = mc.GetIntervalConfiance();
	}
}