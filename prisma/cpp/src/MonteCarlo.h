#pragma once
#include "pnl/pnl_random.h" 
#include "BlackSchole.h"
#include "Prisma.h"
#include "Call.h"// to delete
#include "Data.h"

//#ifdef METHODES_EXPORTS  
//#define METHODES_API __declspec(dllexport)   
//#else  
//#define METHODES_API __declspec(dllimport)   
//#endif  
using namespace Models;
using namespace Produits;
namespace Methodes
{
	class  MonteCarlo
	{
	public:

		void SimpleMonteCarlo(BlackScholeModel* model, Produit* produit, Data data);

		void MonteCarloWithPast(BlackScholeModel* model, Produit* produit, Data data, PnlMat* past =NULL, int t=0);

		MonteCarlo(int n, double r, int T, double thetha, double S0);
		~MonteCarlo();

		double GetPrice();
		double GetIntervalConfiance();
		void fillPaths(BlackScholeModel * model, PnlMat* Paths, PnlMat * past, PnlMat* Gmat, PnlVectInt* tvect, int sousjacentSize, SousJacent* sousjacent, Data data, int t);

		void GetDelta(PnlVect* Delta, BlackScholeModel * model, Produit* produit, Data data, PnlMat* prices, int n, int t);
		void GenerateNextDatePrices(BlackScholeModel * model, Produit* produit, Data data, int t);
	private:
		double GetRightSigma(SousJacent sj, Data data);

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