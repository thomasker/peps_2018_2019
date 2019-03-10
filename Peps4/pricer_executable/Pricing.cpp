#include "Pricing.h"


namespace Pricing
{
	void PricingOptions::pricingCall()
	{
		cout << "\n \n \n Pricing d un Call: " << "\n";
		double K = 110;
		double S0 = 100;
		date start = date(2, 1, 2002);
		date end = date(6, 1, 2002);
		PnlVectInt* dates = pnl_vect_int_create(1);
		TimeManager::fillOpenDates(dates, start, end);
		int T = pnl_vect_int_get(dates,dates->size -1);
		int n = 50000;
		double r = 0.1;

		double sigma = 0.01;
		PnlVect* sigmaVect = pnl_vect_create_from_double(1, sigma);

		Call* option = new Call(K, start, end);
		BlackScholeModel* BnS = new BlackScholeModel();
		MonteCarlo  mc = MonteCarlo(n,r,T, 0,S0);
		Data data = Data(option);
		data.SetVolatilitySouJacent(sigmaVect);

		mc.SimpleMonteCarlo(BnS,option,data);


		double Price = mc.GetPrice();
		double IC = mc.GetIntervalConfiance();
		cout << "Prix via le Pricer : " << Price << "  IC via le Pricer : " << IC << "\n";

		double* price = new double(); 
		*price = 0;
		double *deltha = new double();
		*deltha = 0;
		double tot = pnl_cf_call_bs(S0, K, T, r, 0, sigma, price, deltha);
		cout << "Prix via pnl : " << *price << "  deltha via pnl : " << *deltha <<"\n";
	}
	void PricingOptions::pricingBasket()
	{
		cout << "\n \n \n Pricing d une option de basket contenant 10 actif independant : " << "\n";
		double K = 110;
		double S0 = 100;
		date start = date(2, 1, 2002);
		date end = date(6, 1, 2002);
		PnlVectInt* dates = pnl_vect_int_create(1);
		TimeManager::fillOpenDates(dates, start, end);
		int T = pnl_vect_int_get(dates, dates->size - 1);
		int n = 50000;
		double r = 0.1;


		Basket* option = new Basket(K, start, end);

		double sigma = 0.01;
		PnlVect* sigmaVect = pnl_vect_create_from_double(option->GetSousjacentsSize(), sigma);
		PnlVect* diag = pnl_vect_create_from_double(option->GetSousjacentsSize(), 1);
		//pnl_vect_print(diag);
		PnlMat* correlation = pnl_mat_create_diag(diag);
		//pnl_mat_print(correlation);
		Data data = Data(option);
		data.SetVolatilitySouJacent(sigmaVect);
		data.SetCorrelations(correlation);


		
		BlackScholeModel* BnS = new BlackScholeModel();

		MonteCarlo  mc = MonteCarlo(n, r, T, 0, S0);
		
		mc.SimpleMonteCarlo(BnS, option, data);

		double Price = mc.GetPrice();
		double IC = mc.GetIntervalConfiance();

		cout << "Prix via le Pricer : " << Price << "IC via le Pricer : " << IC << "\n";
	}
	void PricingOptions::pricingPrisma()
	{
		cout << "\n \n \n Pricing du produit Prisma: " << "\n";
		cout << "ps1 : les volatilite des 15 actif sont defini a 0.01 " << "\n";
		cout << "ps2 : les 15 actif sont independant " << "\n";
		double S0 = 100;
		int n = 500;
		double r = 0.1/365;
		double thetha = 0.01;

		Prisma* option = new Prisma();

		double sigma = 0.01;
		PnlVect* sigmaVect = pnl_vect_create_from_double(option->GetSousjacentsSize(), sigma);
		PnlVect* sigmaCurency = pnl_vect_create_from_double(option->GetNbForeignCurrency(), sigma);
		PnlVect* diag = pnl_vect_create_from_double(option->GetSousjacentsSize()+ option->GetNbForeignCurrency(), 1);
		
		PnlMat* correlation = pnl_mat_create_diag(diag);
		
		Data data = Data(option);
		data.SetVolatilitySouJacent(sigmaVect);
		data.SetVolatilityCurency(sigmaCurency);
		data.SetCorrelations(correlation);


		int T = option->GetMaturity();
		BlackScholeModel* BnS = new BlackScholeModel();
		MonteCarlo  mc = MonteCarlo(n, r, T, thetha, S0);

		mc.SimpleMonteCarlo(BnS, option, data);
		double Price = mc.GetPrice();
		double IC = mc.GetIntervalConfiance();
		cout << "Prix via le Pricer : " << Price << "IC via le Pricer : " << IC << "\n";
	}
	void PricingOptions::deltaCall()
	{
		cout << "\n \n \n hedging d un Call: " << "\n";
		double K = 110;
		double S0 = 100;
		date start = date(2, 1, 2002);
		date end = date(6, 1, 2002);
		PnlVectInt* dates = pnl_vect_int_create(1);
		TimeManager::fillOpenDates(dates, start, end);
		int T = pnl_vect_int_get(dates, dates->size - 1);
		int n = 50000;
		double r = 0.01;

		double sigma = 0.01;
		PnlVect* sigmaVect = pnl_vect_create_from_double(1, sigma);

		Call* option = new Call(K, start, end);
		BlackScholeModel* BnS = new BlackScholeModel();
		MonteCarlo  mc = MonteCarlo(n, r, T, 0.2, S0);
		Data data = Data(option);
		data.SetVolatilitySouJacent(sigmaVect);

		PnlVect* Deltha = pnl_vect_create_from_double(option->GetSousjacentsSize(), 0);
		PnlMat* prix = pnl_mat_create(option->GetSousjacentsSize(), option->GetDates()->size);
		pnl_mat_set_all(prix, 100);

		mc.GetDelta(Deltha, BnS, option, data, prix, 100000, 0);
		cout << "prix  : "<< mc .GetPrice()<<" et les deltha via le pricer : " << "\n";
		pnl_vect_print(Deltha);
		double* price = new double();
		*price = 0;
		double *deltha = new double();
		*deltha = 0;
		double tot = pnl_cf_call_bs(S0, K, T, r, 0, sigma, price, deltha);
		cout << "Prix via pnl : " << *price << "  deltha via pnl : " << *deltha << "\n";
 }

	void PricingOptions::deltaBasket() {
		cout << "\n \n \n hedging d une option de basket contenant 10 actif independant : " << "\n";
		double K = 110;
		double S0 = 100;
		date start = date(2, 1, 2002);
		date end = date(6, 1, 2002);
		PnlVectInt* dates = pnl_vect_int_create(1);
		TimeManager::fillOpenDates(dates, start, end);
		int T = pnl_vect_int_get(dates, dates->size - 1);
		int n = 1000;
		double r = 0.1;


		Basket* option = new Basket(K, start, end);

		double sigma = 0.01;
		PnlVect* sigmaVect = pnl_vect_create_from_double(option->GetSousjacentsSize(), sigma);
		PnlVect* diag = pnl_vect_create_from_double(option->GetSousjacentsSize(), 1);
		//pnl_vect_print(diag);
		PnlMat* correlation = pnl_mat_create_diag(diag);
		//pnl_mat_print(correlation);
		Data data = Data(option);
		data.SetVolatilitySouJacent(sigmaVect);
		data.SetCorrelations(correlation);



		BlackScholeModel* BnS = new BlackScholeModel();

		MonteCarlo  mc = MonteCarlo(n, r, T, 0, S0);

		PnlVect* Deltha = pnl_vect_create_from_double(option->GetSousjacentsSize(), 0);
		PnlMat* prix = pnl_mat_create(option->GetSousjacentsSize(), option->GetDates()->size);
		pnl_mat_set_all(prix, 100);

		mc.GetDelta(Deltha, BnS, option, data, prix, 100000, 0);
		pnl_vect_print(Deltha);

		/*cout << "Prix via le Pricer : " << Price << "IC via le Pricer : " << IC << "\n";*/
	}
	void PricingOptions::deltaPrisma()
	{
		cout << "\n \n \n hedging du produit Prisma: " << "\n";
		double S0 = 100;
		int n = 1000;
		double r = 0.1 / 365;
		double thetha = 0.01;

		Prisma* option = new Prisma();

		double sigma = 0.01;
		PnlVect* sigmaVect = pnl_vect_create_from_double(option->GetSousjacentsSize(), sigma);
		PnlVect* sigmaCurency = pnl_vect_create_from_double(option->GetNbForeignCurrency(), sigma);
		PnlVect* diag = pnl_vect_create_from_double(option->GetSousjacentsSize() + option->GetNbForeignCurrency(), 1);

		PnlMat* correlation = pnl_mat_create_diag(diag);

		Data data = Data(option);
		data.SetVolatilitySouJacent(sigmaVect);
		data.SetVolatilityCurency(sigmaCurency);
		data.SetCorrelations(correlation);


		int T = option->GetMaturity();
		BlackScholeModel* BnS = new BlackScholeModel();
		MonteCarlo  mc = MonteCarlo(n, r, T, thetha, S0);

		PnlVect* Deltha = pnl_vect_create_from_double(option->GetSousjacentsSize(),0);
		PnlMat* prix = pnl_mat_create(option->GetSousjacentsSize(), option->GetDates()->size);
		pnl_mat_set_all(prix, 100);

		mc.GetDelta(Deltha,BnS, option, data, prix, n,0);
		cout << "les deltha via le pricer : "  << "\n";
		pnl_vect_print(Deltha);
	}
	void PricingOptions::hedgeCall()
	{
		//initialis� 
		cout << "\n \n \n hedging d un Call: " << "\n";
		//Caracteristique du call 
		double K = 105;
		double S0 = 100;
		double r = 0.02;
		double sigma = 0.01;
		date start = date(2, 1, 2002);
		date end = date(6, 1, 2002);

		// initialisation des don� utile pour l'evaluation du prix 
		int n = 10000;
		PnlVectInt* dates = pnl_vect_int_create(1);
		TimeManager::fillOpenDates(dates, start, end);
		int T = pnl_vect_int_get(dates, dates->size - 1);
		PnlVect* sigmaVect = pnl_vect_create_from_double(1, sigma);

		//creation des objects 
		Call* option = new Call(K, start, end);
		BlackScholeModel* BnS = new BlackScholeModel();
		MonteCarlo  mc = MonteCarlo(n, r, T, 0.2, S0);
		Data data = Data(option);


		data.SetVolatilitySouJacent(sigmaVect);
		/*ici c est des truc que je peut integr� a data */
		PnlVect* Deltha = pnl_vect_create_from_double(option->GetSousjacentsSize(), 0);
		PnlVect* fisrtPrice = pnl_vect_create_from_double(option->GetSousjacentsSize(), S0);
		pnl_vect_print(fisrtPrice);
		//pnl_mat_print(data.SousJacentsPrice);
		pnl_mat_set_row(data.SousJacentsPrice, fisrtPrice, 0);
		pnl_vect_set(data.HedgePriceHistory, 0, 3);

		
		// 1ere date 
		// calcul� le prix , et les deltha,
		mc.MonteCarloWithPast(BnS, option, data, data.SousJacentsPrice, 0);
		pnl_vect_set(data.ProductPriceHistory, 0, mc.GetPrice());
		mc.GetDelta(Deltha,BnS, option, data, pnl_mat_transpose(data.SousJacentsPrice),n,0);
		// cr��e le portefeuille de couverture 
		pnl_vect_set(data.Hedge,0, GET(data.HedgePriceHistory, 0)*GET(Deltha,0)/S0);// d*P(t-1)*/S(t-1)
		pnl_vect_set(data.Hedge, 1, GET(data.HedgePriceHistory,0)*(1-GET(Deltha, 0))); //(1-d)*P(t-1)
		double prix = 0.;
		
		
		//autres date
		for (int nt = 1; nt < option->GetDates()->size - 1; nt++) {
			// evolution des prix des sous jacents
			mc.GenerateNextDatePrices(BnS, option,  data, nt);
			/*cout << "avec nt ="<<nt<<" on a : \n";
			pnl_mat_print(data.SousJacentsPrice);*/
			//  calcul du prix , et des deltha et le prix du portefeille de couverture
			mc.MonteCarloWithPast(BnS, option, data, pnl_mat_transpose(data.SousJacentsPrice), nt);
			pnl_vect_set(data.ProductPriceHistory, nt, mc.GetPrice());
			mc.GetDelta(Deltha, BnS, option, data, pnl_mat_transpose(data.SousJacentsPrice), n, nt);
			//P(t) = d*P(t-1)/S(t-1) * S(t) + (1-d)*P(t-1) * r
			prix = GET(data.Hedge, 0)* MGET(data.SousJacentsPrice, nt, 0)+ GET(data.Hedge, 1)*GET(data.Rates,0);
			pnl_vect_set(data.HedgePriceHistory, nt, prix);
			// cr��e le portefeuille de couverture
			pnl_vect_set(data.Hedge, 0, GET(Deltha, 0) * prix / MGET(data.SousJacentsPrice, nt, 0));// d*P(t)*/S(t)
			pnl_vect_set(data.Hedge, 1, (1 - GET(Deltha, 0) * prix)); //(1-d)*P(t)
		}
		//dernierre date 
		// evolution des prix des sous jacents
		int lastDay = option->GetDates()->size - 1;
		mc.GenerateNextDatePrices(BnS, option, data, lastDay);
		// calcule du prix et le prix du portefeille de couverture
		double finalPrice = option->GetPrice(pnl_mat_transpose(data.SousJacentsPrice), GET(data.Rates, 0), option->GetDates()->size - 1);
		pnl_vect_set(data.ProductPriceHistory , lastDay, finalPrice);
		prix = GET(data.Hedge, 0)* MGET(data.SousJacentsPrice, lastDay,0) + GET(data.Hedge, 1)*GET(data.Rates, 0);
		pnl_vect_set(data.HedgePriceHistory, lastDay, prix);
		// calcul du pnl final 
		double pnl = prix - finalPrice;
		cout << "l evolution du prix du sous jacent : \n";
		pnl_mat_print(data.SousJacentsPrice);
		cout << "l evolution du prix du produit : \n";
		pnl_vect_print(data.ProductPriceHistory);
		cout << "l evolution du prix de la couverture : \n";
		pnl_vect_print(data.HedgePriceHistory);
		cout << "le pnl : " << pnl << "\n";
	}
	void PricingOptions::dayToDayPrisma()
	{// ATENTION NE MARCHE PAS LE CODE A EVOLU� DEPUIS 
		double S0 = 100;
		int n = 10;
		double r = 0.1 / 365;
		double thetha = 0.01;

		Prisma* option = new Prisma();

		int SjSize = option->GetSousjacentsSize();

		double sigma = 0.01;
		PnlVect* sigmaVect = pnl_vect_create_from_double(SjSize, sigma);
		PnlVect* sigmaCurency = pnl_vect_create_from_double(option->GetNbForeignCurrency(), (double)sigma);
		PnlVect* diag = pnl_vect_create_from_double(SjSize + option->GetNbForeignCurrency(), 1.);

		PnlMat* correlation = pnl_mat_create_diag(diag);

		Data data = Data(option);
		data.SetVolatilitySouJacent(sigmaVect);
		data.SetVolatilityCurency(sigmaCurency);
		data.SetCorrelations(correlation);


		int T = option->GetMaturity();
		BlackScholeModel* BnS = new BlackScholeModel();
		MonteCarlo  mc = MonteCarlo(n, r, T, thetha, S0);

		int nbActif = SjSize + option->GetNbForeignCurrency()+1;
		double hedgePrice = 0.;
		PnlVect* PnL = pnl_vect_create_from_double(option->GetDates()->size, 0);
		PnlVect* Deltha = pnl_vect_create_from_double(SjSize, 0);
		PnlVect* PastPrices = pnl_vect_create_from_double(SjSize, 0);
		PnlVect* Prices = pnl_vect_create_from_double(SjSize, 100);// hipotese de prix de debut a 100
		PnlMat* prix = pnl_mat_create(SjSize, option->GetDates()->size);
		pnl_mat_set_row(prix, Prices,0);

		pnl_vect_set(data.HedgePriceHistory, 0, 100);
		//calcul des deltha en t = 0
		mc.GetDelta(Deltha, BnS, option, data, prix, 100, 0);
		data.SetDelthaHistoricSousJacent(Deltha, 0);
		pnl_mat_get_row(PastPrices, prix, 0);
		
		//creation de la couverture en t = 0
		pnl_vect_set_all(data.Hedge, 0.);
		for (int k = 0; k < SjSize; k++) {// attention monaie , peut ne pas etre bien rang� , securit� a rajout�
			double tmp = GET(Deltha, k);
			int monaie = option->GetSousjacents()[k].monaie;
			pnl_vect_set(data.Hedge, k, tmp);
			pnl_vect_set(data.Hedge, SjSize + monaie, GET(data.Hedge, SjSize + monaie) + 1 - tmp);
		}
		for (int t = 1; t < option->GetDates()->size; t++) {
			// generation de l evolution des prix
			//mc.GenerateNextDatePrices(Prices, PastPrices, BnS, option, t + 1, data);
			data.SetPriceHistoricSousJacent(Prices, t + 1);
			pnl_mat_set_row(prix, Prices, t);
			// prend les ancien deltha et calcul le prix de la couverture
			hedgePrice = 0;
			for (int i = 0; i < SjSize; i++) {
				hedgePrice += GET(Prices, i)*GET(data.Hedge, i);
			}
			for (int j = 0; j < option->GetNbForeignCurrency() + 1; j++) {
				hedgePrice += GET(data.Hedge, SjSize + j)*GET(data.Rates, j);
			}
			pnl_vect_set(data.HedgePriceHistory, t, hedgePrice);
			// calcul du prix de l actif 
			mc.MonteCarloWithPast(BnS, option, data, prix, t);
			// calcul du pnl
			pnl_vect_set(PnL, t, hedgePrice -mc.GetPrice() );

			//calcul des deltha
			mc.GetDelta(Deltha, BnS, option, data, prix, 100, t);
			data.SetDelthaHistoricSousJacent(Deltha, t);
			pnl_mat_get_row(PastPrices, prix, t);
			//creation de la couverture 
			pnl_vect_set_all(data.Hedge, 0.);
			for (int k = 0; k < SjSize; k++) {// attention monaie , peut ne pas etre bien rang� , securit� a rajout�
				double tmp = GET(Deltha, k);
				int monaie = option->GetSousjacents()[k].monaie;
				pnl_vect_set(data.Hedge, k, tmp);				
				pnl_vect_set(data.Hedge, SjSize+ monaie, GET(data.Hedge, SjSize + monaie) + 1 - tmp);
			}
		}
	}
}