#include "stdafx.h"
#include "Prisma.h"
#include "Date.h"
#include "TimeManager.h"
#include <math.h>

using namespace Outils;

namespace Produits
{
	double Prisma::GetPrice(PnlMat * paths, double r, int t)
	{
		return  GetPayOff(paths)*exp(-r * (t - _T)) + Dividendes(r, t);
	}

	double Prisma::GetPayOff(PnlMat* paths)
	{
		double prime = GuaranteedRefund;
		bool continu = true;

		for (int i = 0; i < QuarterlyObservationDates->size; i++)
		{
			for (int j = 0; j < paths->n; j++)
			{
				if (pnl_mat_get(paths, j, (int)pnl_vect_int_get(QuarterlyObservationDates, i)) < 60)
				{
					continu = false;
				}
			}
			if (!continu)
			{
				break;
			}
			prime += FixedPremium;
		}
		double sum = 0;
		double S0 = 0;
		for (int i = 0; i < paths->n; i++)
		{
			S0 = pnl_mat_get(paths, i, 0);
			for (int j = 0; j < AnnualObservationDates->size; j++)
			{
				sum += (pnl_mat_get(paths, i, (int)pnl_vect_int_get(AnnualObservationDates, j)) - S0) / S0;
			}
		}
		if (sum > 0)
		{
			prime += Turnout * 100 * sum / (paths->n * AnnualObservationDates->size);
		}
		return prime;
	}

	double Prisma::Dividendes(double r, int day)
	{
		double sum = 0;
		double tmp = 0.;
		for (int i = 0; i < DividendesDates->size; i++) {
			tmp = pnl_vect_int_get(DividendesDates, i);
			if (day < tmp) {
				sum += DividendAmont * exp(-r * (tmp - day));
			}
		}
		return  sum;
	}

	PnlVectInt * Prisma::GetDates()
	{
		return Dates;
	}

	int Prisma::GetMaturity()
	{
		return _T;
	}

	SousJacent * Prisma::GetSousjacents()
	{
		return sousJacents;
	}

	int Prisma::GetSousjacentsSize()
	{
		return sousJacentsSize;
	}

	int Prisma::GetNbForeignCurrency()
	{
		list<curency> curencyList = {};
		for (int i = 0; i < sousJacentsSize; i++) {
			if (!(std::find(curencyList.begin(), curencyList.end(), sousJacents[i].monaie) != curencyList.end())) {
				curencyList.push_back(sousJacents[i].monaie);
			}
		}
		return (int)curencyList.size();
	}

	Prisma::Prisma(){
		date  start = date(3, 10, 2002, Mercredi);
		date  end = date(27, 12, 2007);
		list <date> quarterlyDateslist = {
			date(30, 6, 2005),
			date(30, 9, 2005),
			date(30, 12, 2005),
			date(30, 3, 2006),
			date(30, 6, 2006),
			date(2, 10, 2006),
			date(2, 1, 2007),
			date(30, 3, 2007),
			date(2, 7, 2007),
			date(1, 10, 2007)
		};
		list<date> annualDateslist = {
			date(30, 9, 2003),
			date(30, 9, 2004),
			date(30, 9, 2005),
			date(2, 10, 2006),
			date(1, 10, 2007)
		};
		list<date> dividendesDateslist = {
			date(25, 9, 2003),
			date(23, 9, 2004),
			date(29, 9, 2005),
			date(28, 9, 2006),
			date(27, 9, 2007)
		};


		SousJacentsConstructor();


		QuarterlyObservationDates = TimeManager::pnlVect_From_Date_List(start, quarterlyDateslist);
		AnnualObservationDates = TimeManager::pnlVect_From_Date_List(start, annualDateslist);
		DividendesDates = TimeManager::pnlVect_From_Date_List(start, dividendesDateslist);

		Dates = pnl_vect_int_create(start.getNbDay(end));
		TimeManager::fillOpenDates(Dates, start, end);
		_T = pnl_vect_int_get(Dates, Dates->size - 1);

	}

	Prisma::Prisma(PnlVectInt* _QuarterlyObservationDates, PnlVectInt* _AnnualObservationDates, PnlVectInt* _DividendesDates, PnlVectInt* _Dates)
{
		SousJacentsConstructor();

		QuarterlyObservationDates = _QuarterlyObservationDates;
		AnnualObservationDates = _AnnualObservationDates;
		DividendesDates = _DividendesDates;
		Dates = _Dates;
		_T = (int)pnl_vect_int_get(_Dates, _Dates->size - 1);
	}
	Prisma::~Prisma() 
	{
		pnl_vect_int_free(&QuarterlyObservationDates);
		pnl_vect_int_free(&AnnualObservationDates);
		pnl_vect_int_free(&DividendesDates);
		pnl_vect_int_free(&Dates);
	}

	void Prisma::SousJacentsConstructor() {

		SousJacent sousjacent;
		char name[] = "LVMH";
		sousjacent.monaie = EUR;
		sousjacent.setName (name);
		sousJacents[0]=sousjacent;

		sousjacent.monaie = EUR;
		char name1[] = "TELECOM ITALIA";
		sousjacent.setName(name1);
		sousJacents[1] = sousjacent;

		sousjacent.monaie = SEK;
		char  name2[] = "NOKIA";
		sousjacent.setName(name2);
		sousJacents[2] = sousjacent;

		sousjacent.monaie = EUR;
		char  name3[] = "DEUTSCHE BANK";
		sousjacent.setName(name3);
		sousJacents[3] = sousjacent;

		sousjacent.monaie = EUR;
		char  name4[] = "AEGON";
		sousjacent.setName(name4);
		sousJacents[4] = sousjacent;

		sousjacent.monaie = USD;
		char  name5[] = "SCHLUMBERGER";
		sousjacent.setName(name5);
		sousJacents[5] = sousjacent;

		sousjacent.monaie = JPY;
		char  name6[] = "NTTDoCoMo";
		sousjacent.setName(name6);
		sousJacents[6] = sousjacent;

		sousjacent.monaie = JPY;
		char  name7[] = "HONDA MOTOR";
		sousjacent.setName(name7);
		sousJacents[7] = sousjacent;

		sousjacent.monaie = JPY;
		char  name8[] = "TOSHIBA";
		sousjacent.setName(name8);
		sousJacents[8] = sousjacent;

		sousjacent.monaie = USD;
		char  name9[] = "PROCTER & GAMBLE";
		sousjacent.setName(name9);
		sousJacents[9] = sousjacent;

		sousjacent.monaie = USD;
		char  name10[] = "BANK OF AMERICA";
		sousjacent.setName(name10);
		sousJacents[10] = sousjacent;

		sousjacent.monaie = EUR;
		char  name11[] = "ENEL";
		sousjacent.setName(name11);
		sousJacents[11] = sousjacent;

		sousjacent.monaie = GBP;
		char  name12[] = "LEGAL & GENERAL";
		sousjacent.setName(name12);
		sousJacents[12] = sousjacent;

		sousjacent.monaie = JPY;
		char  name13[] = "BRIDGESTONE";
		sousjacent.setName(name13);
		sousJacents[13] = sousjacent;

		sousjacent.monaie = EUR;
		char  name14[] = "AIR LQUIDE";
		sousjacent.setName(name14);
		sousJacents[14] = sousjacent;

		sousjacent.monaie = EUR;
		char  name15[] = "GENERALI";
		sousjacent.setName(name15);
		sousJacents[15] = sousjacent;
	}
}