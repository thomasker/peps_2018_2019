#pragma once
#include "pnl/pnl_random.h" 
#include "BlackSchole.h"
#include "Prisma.h"
#include "Call.h"// to delete
#include "Data.h"
#include "ExportDll.h"

//#ifdef METHODES_EXPORTS  
//#define METHODES_API __declspec(dllexport)   
//#else  
//#define METHODES_API __declspec(dllimport)   
//#endif  
using namespace Models;
using namespace Produits;
namespace Methodes
{
	class PRICER_API MonteCarlo
	{
	public:

		void SimpleMonteCarlo(BlackScholeModel* model, Produit* produit, Data data);

		MonteCarlo(int n, double r, int T, double thetha, double S0);
		~MonteCarlo();

		double GetPrice();
		double GetIntervalConfiance();

		void GetDelta(PnlVect* Delta, Produit* produit, PnlMat* prices, int n, double r, int maturite, int t);

	private:
		double GetRightSigma(SousJacent sj, Data data, int date);

		void GenerateRandVect(PnlVect* G, int n, PnlRng* rng = NULL);
		void GenerateMBSVect(PnlVect* W, int n, PnlRng *rng);

		PnlRng* _rng;

		int _n;		//taille de l'estimation 
		double _r;
		double _thetha;
		double _S0;
		int _T;

		double prix;
		double intervalConfiance;
	};


}