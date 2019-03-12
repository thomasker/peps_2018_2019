#pragma once

#include "pnl/pnl_matrix.h"
#include <iostream> 
#include <list> 
#include <iterator> 

#include "Date.h"

using namespace std;

namespace Outils
{
	
	class  TimeManager
	{
	public:
		/*
		* créé un pnlvect de la taille de la liste de date
		* qui contient la difference en jour entre la start date et chaque element de la liste de date
		*/
		static void pnlVect_From_Date_List(PnlVectInt* dateVect, date startDate, list<date> dateList);

		/* remplis le vecteur dates des jours ouverts, pour chaque n eme element du vecteur , 
		* est enregistré  le nombre de jour d ecart entre start et le n eme jour ouvert 
		* start et end sont inclus , si il sont des jours ouvré
		*/
		static void fillOpenDates(PnlVectInt* dates, date  start, date  end);
	};

}