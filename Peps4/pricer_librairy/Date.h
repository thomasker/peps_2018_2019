#pragma once
#include "ExportDll.h"

#include <list> 
#include "JourSemaine.h"

using namespace std;

namespace Outils
{

	struct PRICER_API date {
		int _jour;
		int _mois;
		int _anne;
		jourSemaine _jourSemaine;
		/*date* _jourferiee;*/
		date(int jour, int mois, int anne, jourSemaine jourSemaine = Lundi);
		date(const date& DateToCopy);
		void addDay();
		bool isWeekend();
		bool isClosedDay();
		int getNbDay(const date toDate);
		bool areEqual(const date & a, const date & b);
		bool operator==(const date& a)const;
		bool operator<(const date& a)const;
		bool operator>(const date& a)const;
	};

}