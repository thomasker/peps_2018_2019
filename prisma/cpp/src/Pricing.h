#pragma once
#include "Call.h"
#include "Basket.h"
#include "BlackSchole.h"
#include "MonteCarlo.h"
#include "pnl/pnl_finance.h"
using namespace Methodes;
using namespace Models;

namespace Pricing 
{
	class PricingOptions
	{
	public:
		static void pricingCall();
		static void pricingBasket();
		static void pricingPrisma();

		static void deltaCall();
		static void deltaBasket();
		static void deltaPrisma();

		static void hedgeCall();
		static void hedgeBasket();
		static string hedgePrisma();

		static void HistoricData();
	private:
		static string  JsonPrisma(Data datas, int nbday);
		static double PriceHedge(Produit* option, Data data, int nt, int dt);
		static void CreatPortfolio(Produit* option, Data data, int nt, PnlVect* Delta);
		static void DividendEvent(Prisma* option, Data data, int nt);
	};
}