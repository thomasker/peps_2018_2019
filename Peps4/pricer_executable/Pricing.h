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
		static void hedgeCall();
		static void hedgePrisma();
		static void dayToDayPrisma();
	};
}