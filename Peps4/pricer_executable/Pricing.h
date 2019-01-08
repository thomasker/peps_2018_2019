#pragma once
#include "Call.h"
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
		static void pricingPrisma();
	};
}