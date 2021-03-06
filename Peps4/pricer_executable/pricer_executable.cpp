// pricer_executable.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include "pch.h"
#include <iostream>

#include "TestDates.h"
#include "TestData.h"
#include "TestTimeManager.h"
#include "TestCorelation.h"
#include "TestProduit.h"
#include "Pricing.h"
#include "FileReader.h"
#include "Volatility.h"
using namespace Test;
using namespace Pricing;
using namespace Outils;
using namespace Pricer;
int main()
{
	/*lecture d un ichier csv et calcul de la volatilité*/
	
	/*FileReader toto = FileReader();
	char tata [] = "C:\\Users\\thomask\\Downloads\\cotations\\csv\\TELECOMITALIA.csv";
	PnlVect* evolution = pnl_vect_create(1);
	PnlVectInt* dates = pnl_vect_int_create(1);
	toto.readFile(evolution, dates,tata);
	double volatility =Volatility::historicVolatility(evolution, dates);
	*/
	//PricingOptions::JsonPrisma();

	//PricingOptions::HistoricData();

	//PricingOptions::hedgeCall();
	//PricingOptions::hedgeBasket();
	PricingOptions::hedgePrisma();

	//PricingOptions::deltaCall();
	//PricingOptions::deltaBasket();
	//PricingOptions::deltaPrisma();

	//PricingOptions::pricingCall();
	//PricingOptions::pricingBasket();
	//PricingOptions::pricingPrisma();
	

	/*TESTS :*/

	/*
	TestData::TestTotal();
	TestDates test = TestDates();
	test.TestTotal();
	TestTimeManager::TestTotal();
	TestProduit::TestTotal();
	*/




	/*TEST non fonctionelles:*/

	/*TestCorelation::TestTotal();*/
	/*TestModel::TestTotal();*/
	/*TestMethode::TestTotal();*/
	
	
}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Conseils pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
