#include "Pricing.h"
#include <sstream>

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
		//initialisé 
		cout << "\n \n \n hedging d un Call: " << "\n";
		//Caracteristique du call 
		double K = 110;
		double S0 = 100;
		double r = 0.04/365;
		double sigma = 0.01;
		date start = date(2, 1, 2002);
		date end = date(2, 6, 2002);

		// initialisation des doné utile pour l'evaluation du prix 
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

		pnl_vect_set(data.ZeroRates, 0, r);
		data.SetVolatilitySouJacent(sigmaVect);
		/*ici c est des truc que je peut integré a data */
		PnlVect* Deltha = pnl_vect_create_from_double(option->GetSousjacentsSize(), 0);
		PnlVect* fisrtPrice = pnl_vect_create_from_double(option->GetSousjacentsSize(), S0);
		pnl_vect_print(fisrtPrice);
		//pnl_mat_print(data.SousJacentsPrice);
		pnl_mat_set_row(data.SousJacentsPrice, fisrtPrice, 0);
		//pnl_vect_set(data.HedgePriceHistory, 0, 3);

		
		// 1ere date 
		// calculé le prix , et les deltha,
		//mc.MonteCarloWithPast(BnS, option, data, data.SousJacentsPrice, 0);
		mc.GetDelta(Deltha, BnS, option, data, pnl_mat_transpose(data.SousJacentsPrice), n, 0);
		pnl_vect_set(data.ProductPriceHistory, 0, mc.GetPrice());
		pnl_vect_set(data.HedgePriceHistory, 0, mc.GetPrice());
		
		// créée le portefeuille de couverture 
		//pnl_vect_set(data.Hedge, 0, GET(data.HedgePriceHistory, 0)*GET(Deltha, 0) / S0);// d*P(t-1)*/S(t-1)
		//pnl_vect_set(data.Hedge, 1, GET(data.HedgePriceHistory, 0)*(1 - GET(Deltha, 0))); //(1-d)*P(t-1)
		pnl_vect_set(data.Hedge, 0, GET(Deltha, 0) * S0);// D*S
		pnl_vect_set(data.Hedge, 1, GET(data.HedgePriceHistory, 0)-(GET(Deltha, 0)*S0)); //P-D*S
		//pnl_vect_print(data.Hedge);
		double prix = 0.;
		
		int dt = 0;
		//autres date
		for (int nt = 1; nt < option->GetDates()->size - 1; nt++) {
			dt = pnl_vect_int_get(option->GetDates(), nt) - pnl_vect_int_get(option->GetDates(), nt - 1);
			// evolution des prix des sous jacents
			mc.GenerateNextDatePrices(BnS, option,  data, nt);
			/*cout << "avec nt ="<<nt<<" on a : \n";
			pnl_mat_print(data.SousJacentsPrice);*/
			//  calcul du prix , et des deltha et le prix du portefeille de couverture
			//mc.MonteCarloWithPast(BnS, option, data, pnl_mat_transpose(data.SousJacentsPrice), nt);
			mc.GetDelta(Deltha, BnS, option, data, pnl_mat_transpose(data.SousJacentsPrice), n, nt);
			pnl_vect_set(data.ProductPriceHistory, nt, mc.GetPrice());
			
			//P(t) = d*P(t-1)/S(t-1) * S(t) + (1-d)*P(t-1) * r
			//cout <<"D1 = "<< GET(data.Hedge, 0)<< " r1 = "<< MGET(data.SousJacentsPrice, nt, 0) / MGET(data.SousJacentsPrice, nt - 1, 0) << " D2 = "<< GET(data.Hedge, 1)  << " r2 = "<< GET(data.Rates, 0) <<"\n";
			prix = GET(data.Hedge, 0)* MGET(data.SousJacentsPrice, nt, 0)/ MGET(data.SousJacentsPrice, nt-1, 0) + GET(data.Hedge, 1)*pow(1+GET(data.ZeroRates,0), dt);
			pnl_vect_set(data.HedgePriceHistory, nt, prix);
			// créée le portefeuille de couverture
			//pnl_vect_set(data.Hedge, 0, GET(Deltha, 0) * prix / MGET(data.SousJacentsPrice, nt, 0));// d*P(t)*/S(t)
			//pnl_vect_set(data.Hedge, 1, (1 - GET(Deltha, 0) * prix)); //(1-d)*P(t)
			pnl_vect_set(data.Hedge, 0, GET(Deltha, 0) * MGET(data.SousJacentsPrice, nt, 0));// D*S
			pnl_vect_set(data.Hedge, 1, prix - GET(Deltha, 0) * MGET(data.SousJacentsPrice, nt, 0)); //P-D*S
		}
		//dernierre date 
		// evolution des prix des sous jacents
		int lastDay = option->GetDates()->size - 1;
		mc.GenerateNextDatePrices(BnS, option, data, lastDay);
		// calcule du prix et le prix du portefeille de couverture
		double finalPrice = option->GetPrice(pnl_mat_transpose(data.SousJacentsPrice), GET(data.ZeroRates, 0), option->GetDates()->size - 1);
		pnl_vect_set(data.ProductPriceHistory , lastDay, finalPrice);
		prix = GET(data.Hedge, 0)* MGET(data.SousJacentsPrice, lastDay,0) / MGET(data.SousJacentsPrice, lastDay - 1, 0) + GET(data.Hedge, 1)*(1+GET(data.ZeroRates, 0));
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
	void PricingOptions::hedgeBasket()
	{
		//initialisé 
		cout << "\n \n \n hedging d un basket: " << "\n";
		//Caracteristique du call 
		double K = 102;
		double S0 = 100;
		double r = 0.0075/365;
		double sigma = 0.01;
		date start = date(2, 1, 2002);
		date end = date(2, 6, 2002);

		// initialisation des doné utile pour l'evaluation du prix 
		int n = 10000;
		PnlVectInt* dates = pnl_vect_int_create(1);
		TimeManager::fillOpenDates(dates, start, end);
		int T = pnl_vect_int_get(dates, dates->size - 1);

		//creation des objects 
		Basket* option = new Basket(K, start, end);
		BlackScholeModel* BnS = new BlackScholeModel();
		MonteCarlo  mc = MonteCarlo(n, r, T, sigma, S0);
		Data data = Data(option);


		PnlVect* sigmaVect = pnl_vect_create_from_double(option->GetSousjacentsSize(), sigma);
		PnlVect* diag = pnl_vect_create_from_double(option->GetSousjacentsSize(), 1);
		PnlMat* correlation = pnl_mat_create_diag(diag);

		
		pnl_vect_set(data.ZeroRates, 0, r);
		data.SetVolatilitySouJacent(sigmaVect);
		data.SetCorrelations(correlation);
		/*ici c est des truc que je peut integré a data */
		PnlVect* Deltha = pnl_vect_create_from_double(option->GetSousjacentsSize(), 0);
		PnlVect* fisrtPrice = pnl_vect_create_from_double(option->GetSousjacentsSize(), S0);
		pnl_vect_print(fisrtPrice);
		//pnl_mat_print(data.SousJacentsPrice);
		pnl_mat_set_row(data.SousJacentsPrice, fisrtPrice, 0);
		//pnl_vect_set(data.HedgePriceHistory, 0, 3);


		// 1ere date 
		// calculé le prix , et les deltha,
		//mc.MonteCarloWithPast(BnS, option, data, data.SousJacentsPrice, 0);
		mc.GetDelta(Deltha, BnS, option, data, pnl_mat_transpose(data.SousJacentsPrice), n, 0);
		pnl_vect_set(data.ProductPriceHistory, 0, mc.GetPrice());
		pnl_vect_set(data.HedgePriceHistory, 0, mc.GetPrice());

		// créée le portefeuille de couverture 
		//pnl_vect_set(data.Hedge, 0, GET(data.HedgePriceHistory, 0)*GET(Deltha, 0) / S0);// d*P(t-1)*/S(t-1)
		//pnl_vect_set(data.Hedge, 1, GET(data.HedgePriceHistory, 0)*(1 - GET(Deltha, 0))); //(1-d)*P(t-1)
		double totalHedge = 0.;
		for (int i = 0; i < Deltha->size-1 ; i++) {
			totalHedge += GET(Deltha, i) * S0;
			pnl_vect_set(data.Hedge, i, GET(Deltha, i) * S0);
		}
		//pnl_vect_set(data.Hedge, 0, GET(Deltha, 0) * S0);// D*S
		pnl_vect_set(data.Hedge, Deltha->size-1, GET(data.HedgePriceHistory, 0) - totalHedge); //P-D*S
		//pnl_vect_print(data.Hedge);
		double prix = 0.;

		int dt = 0;
		//autres date
		for (int nt = 1; nt < option->GetDates()->size - 1; nt++) {
			dt = pnl_vect_int_get(option->GetDates(), nt) - pnl_vect_int_get(option->GetDates(), nt - 1);
			// evolution des prix des sous jacents
			mc.GenerateNextDatePrices(BnS, option, data, nt);
			/*cout << "avec nt ="<<nt<<" on a : \n";
			pnl_mat_print(data.SousJacentsPrice);*/
			//  calcul du prix , et des deltha et le prix du portefeille de couverture
			//mc.MonteCarloWithPast(BnS, option, data, pnl_mat_transpose(data.SousJacentsPrice), nt);
			mc.GetDelta(Deltha, BnS, option, data, pnl_mat_transpose(data.SousJacentsPrice), n, nt);
			pnl_vect_set(data.ProductPriceHistory, nt, mc.GetPrice());

			//P(t) = d*P(t-1)/S(t-1) * S(t) + (1-d)*P(t-1) * r
			//cout <<"D1 = "<< GET(data.Hedge, 0)<< " r1 = "<< MGET(data.SousJacentsPrice, nt, 0) / MGET(data.SousJacentsPrice, nt - 1, 0) << " D2 = "<< GET(data.Hedge, 1)  << " r2 = "<< GET(data.Rates, 0) <<"\n";
			//prix = GET(data.Hedge, 0)* MGET(data.SousJacentsPrice, nt, 0) / MGET(data.SousJacentsPrice, nt - 1, 0) + GET(data.Hedge, 1)*pow(1 + GET(data.Rates, 0), dt);
			prix = 0;
			for (int i = 0; i < Deltha->size - 1; i++) {
				prix += GET(data.Hedge, i)* MGET(data.SousJacentsPrice, nt, i) / MGET(data.SousJacentsPrice, nt - 1, i);
			}
			prix += GET(data.Hedge, Deltha->size - 1)*pow(1 + GET(data.ZeroRates, 0), dt);
			pnl_vect_set(data.HedgePriceHistory, nt, prix);
			// créée le portefeuille de couverture
			//pnl_vect_set(data.Hedge, 0, GET(Deltha, 0) * prix / MGET(data.SousJacentsPrice, nt, 0));// d*P(t)*/S(t)
			//pnl_vect_set(data.Hedge, 1, (1 - GET(Deltha, 0) * prix)); //(1-d)*P(t)
			//pnl_vect_set(data.Hedge, 0, GET(Deltha, 0) * MGET(data.SousJacentsPrice, nt, 0));// D*S
			totalHedge = 0.;
			for (int i = 0; i < Deltha->size-1; i++) {
				totalHedge += GET(Deltha, i) * MGET(data.SousJacentsPrice, nt, i);
				pnl_vect_set(data.Hedge, i, GET(Deltha, i) * MGET(data.SousJacentsPrice, nt, i));
			}
			pnl_vect_set(data.Hedge, Deltha->size-1, prix - totalHedge); //P-D*S
		}
		//dernierre date 
		// evolution des prix des sous jacents
		int lastDay = option->GetDates()->size - 1;
		mc.GenerateNextDatePrices(BnS, option, data, lastDay);
		// calcule du prix et le prix du portefeille de couverture
		double finalPrice = option->GetPrice(pnl_mat_transpose(data.SousJacentsPrice), GET(data.ZeroRates, 0), option->GetDates()->size - 1);
		pnl_vect_set(data.ProductPriceHistory, lastDay, finalPrice);
		//prix = GET(data.Hedge, 0)* MGET(data.SousJacentsPrice, lastDay, 0) / MGET(data.SousJacentsPrice, lastDay - 1, 0) + GET(data.Hedge, 1)*(1 + GET(data.Rates, 0));
		prix = 0;
		for (int i = 0; i < Deltha->size - 1; i++) {
			prix += GET(data.Hedge, i)* MGET(data.SousJacentsPrice, lastDay, i) / MGET(data.SousJacentsPrice, lastDay - 1, i);
		}
		prix += GET(data.Hedge, Deltha->size - 1)*pow(1 + GET(data.ZeroRates, 0), dt);
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
	string PricingOptions::hedgePrisma()
	{
		//initialisé 
		cout << "\n \n \n hedging de prisma: " << "\n";
		//Caracteristique du call 
		double K = 102;
		double S0 = 100;
		double r = 0.1 / 365;
		double sigma = 0.01;

		// initialisation des doné utile pour l'evaluation du prix 
		int n = 1000;
		int T = 0;//inutile ici 

		//creation des objects 
		Prisma* option = new Prisma();
		BlackScholeModel* BnS = new BlackScholeModel();
		MonteCarlo  mc = MonteCarlo(n, r, T, sigma, S0);
		Data data = Data(option);


		PnlVect* sigmaVect = pnl_vect_create_from_double(option->GetSousjacentsSize(), sigma);
		PnlVect* diag = pnl_vect_create_from_double(option->GetSousjacentsSize() + option->GetNbForeignCurrency(), 1);
		PnlMat* correlation = pnl_mat_create_diag(diag);


		pnl_vect_set(data.ZeroRates, 0, r);
		data.SetVolatilitySouJacent(sigmaVect);
		data.SetCorrelations(correlation);
		/*ici c est des truc que je peut integré a data */
		PnlVect* Delta = pnl_vect_create_from_double(option->GetSousjacentsSize()+ option->GetNbForeignCurrency(), 0);
		PnlVect* fisrtPrice = pnl_vect_create_from_double(option->GetSousjacentsSize(), S0);
		pnl_mat_set_row(data.SousJacentsPrice, fisrtPrice, 0);


		// 1ere date 
		// calculé le prix , et les delta,
		mc.GetDelta(Delta, BnS, option, data, pnl_mat_transpose(data.SousJacentsPrice), n, 0);
		pnl_vect_set(data.ProductPriceHistory, 0, mc.GetPrice());
		pnl_vect_set(data.HedgePriceHistory, 0, mc.GetPrice());

		// créée le portefeuille de couverture 
		double totalHedge = 0.;
		double tmp = 0.;
		CreatPortfolio(option, data, 0, Delta);
		int nbday = 5;


		double prix = 0.;
		int dt = 0;
		//autres date
		for (int nt = 1; nt < nbday; nt++) {//option->GetDates()->size - 1
			dt = pnl_vect_int_get(option->GetDates(), nt) - pnl_vect_int_get(option->GetDates(), nt - 1);
			// evolution des prix des sous jacents
			mc.GenerateNextDatePrices(BnS, option, data, nt);
			// ICI FAIR UN TRUC POUR L EVOLUTON DES TAUX ............................................................................
			//  calcul du prix , et des delta et le prix du portefeille de couverture
			mc.GetDelta(Delta, BnS, option, data, pnl_mat_transpose(data.SousJacentsPrice), n, nt);
			pnl_vect_set(data.ProductPriceHistory, nt, mc.GetPrice());
			//cout << "le nouveau prix : "<< mc.GetPrice() <<"\n";
			PriceHedge(option, data, nt, dt);
			// créée le portefeuille de couverture
			CreatPortfolio( option,  data,  nt, Delta);

			/*test */
			/*prix = 0;
			for (int i = 0; i < data.Hedge->size; i++) {
				monaieIndice = option->GetSousjacents()[i].monaie;
				prix += GET(data.Hedge, i);
			}
			cout << "la couverture de aujourdhui vaut: "<< prix <<" \n";*/
			//pnl_vect_print(data.Hedge);
		}
		//dernierre date 
		// evolution des prix des sous jacents
		int lastDay = option->GetDates()->size - 1;
		dt = pnl_vect_int_get(option->GetDates(), lastDay) - pnl_vect_int_get(option->GetDates(), lastDay - 1);
		mc.GenerateNextDatePrices(BnS, option, data, lastDay);
		// calcule du prix et le prix du portefeille de couverture
		double finalPrice = option->GetPrice(pnl_mat_transpose(data.SousJacentsPrice), GET(data.ZeroRates, 0), option->GetDates()->size - 1);
		pnl_vect_set(data.ProductPriceHistory, lastDay, finalPrice);

		prix = PriceHedge( option,  data, lastDay,  dt);

		// calcul du pnl final 
		double pnl = prix - finalPrice;
		cout << "le pnl : " << pnl << "\n";
		return JsonPrisma(data, nbday);
	}
	string PricingOptions::JsonPrisma(Data datas, int nbday)
	{
		std::ostringstream strs;
		strs << "{ \n";
		strs << "\"SousJacentsPrice\" : [\n";
		std::string str = strs.str();
		for (int i = 0; i < nbday; i++) {
			if (i != 0) {
				strs << ",\n";
			}
			strs << "	[";
			for (int j = 0; j < datas.SousJacentsPrice->n ; j++) {
				if (j != 0) {
					strs << " , ";
				}
				strs << MGET(datas.SousJacentsPrice, i, j);
				
			}
			strs << "]";
		}
		strs << "\n],\n";
		strs <<"\"ProductPriceHistory\" : [";
		for (int i = 0; i < nbday; i++) {
			if (i != 0) {
				strs << " , ";
			}
			strs << GET(datas.ProductPriceHistory, i);
		}
		strs <<"],\n";
		strs <<"\"HedgePriceHistory\"  : [";
		for (int i = 0; i < nbday; i++) {
			if (i != 0) {
				strs << " , ";
			}
			strs << GET(datas.HedgePriceHistory, i);
		}
		strs <<"]\n";
		strs << "}";
		string x_str = strs.str();
		cout << x_str;
		return x_str;
	}
	double PricingOptions::PriceHedge(Produit* option, Data data, int nt ,int dt) {
		double prix = 0;
		int monaieIndice = 0;
		for (int i = 0; i < option->GetSousjacentsSize(); i++) {
			monaieIndice = option->GetSousjacents()[i].monaie;
			prix += GET(data.Hedge, i)* MGET(data.SousJacentsPrice, nt, i) / MGET(data.SousJacentsPrice, nt - 1, i)* GET(data.CurencyRates, monaieIndice);
		}
		for (int i = 0; i <= option->GetNbForeignCurrency(); i++) {
			prix += GET(data.Hedge, option->GetSousjacentsSize() + i) *pow(1 + GET(data.ZeroRates, i), dt)* GET(data.CurencyRates, i);
		}
		pnl_vect_set(data.HedgePriceHistory, nt, prix);
		return prix;
	}

	void PricingOptions::CreatPortfolio(Produit* option, Data data, int nt, PnlVect* Delta) {
		double totalHedge = 0.;
		int monaieIndice = 0;
		double tmp = 0.;
		PnlVect* totalmonaie = pnl_vect_create_from_double(option->GetNbForeignCurrency()+1,0.);
		for (int i = 0; i < option->GetSousjacentsSize(); i++) {
			monaieIndice = option->GetSousjacents()[i].monaie;
			tmp = GET(Delta, i) * MGET(data.SousJacentsPrice, nt, i);// ici on par de l hipotese que l information du rate est deja comprise dans le delta
			totalHedge += tmp * GET(data.CurencyRates, monaieIndice);
			pnl_vect_set(data.Hedge, i, tmp);
			pnl_vect_set(totalmonaie, monaieIndice, GET(totalmonaie, monaieIndice) + tmp);
		}
		//hedge  : argent actif en monaie etrangere  : zc euro : zc erangé monaie etrangere 
		for (int i = 1; i <= option->GetNbForeignCurrency(); i++) {
			tmp = GET(Delta, option->GetSousjacentsSize() + i - 1) - GET(totalmonaie, i);// delta *1 - total monaie etrangere
			totalHedge += tmp * GET(data.CurencyRates, i);
			pnl_vect_set(data.Hedge, option->GetSousjacentsSize() + i, tmp);
		}

		pnl_vect_set(data.Hedge, option->GetSousjacentsSize(), GET(data.HedgePriceHistory, nt) - totalHedge);

		pnl_vect_free(&totalmonaie);
	}
}