#pragma once

//#ifdef PRODUITS_EXPORTS  
//#define PRICER_API __declspec(dllexport)   
//#else  
//#define PRICER_API __declspec(dllimport)   
//#endif  



#include"sousjacent.h"

#include "pnl/pnl_matrix.h"

namespace Produits
{
	class  Produit
	{
	public:
		virtual double GetPayOff(PnlMat* paths) = 0;
		virtual double GetPrice(PnlMat* paths, double r, int t) = 0;
		virtual PnlVectInt* GetDates() = 0;
		virtual SousJacent* GetSousjacents() = 0;
		virtual int GetSousjacentsSize() = 0;
		virtual int GetNbForeignCurrency() = 0;

	protected:
		int _T;
	};
}