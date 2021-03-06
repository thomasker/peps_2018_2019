#include "stdafx.h"
#include "Prisma.h"
#include "Date.h"
#include "TimeManager.h"
#include <math.h>
#include <exception>

using namespace Outils;

namespace Produits
{
	double Prisma::GetPrice(PnlMat * paths, double r, int nt)
	{
		int t = pnl_vect_int_get(Dates, nt);
		return  GetPayOff(paths)*exp(-r * (_T -t )) + Dividendes(r, t);
	}

	double Prisma::GetPayOff(PnlMat* paths)//les paths contienne maintenant les prix et les taux
	{
		curency mmonaiejs = EUR;
		if (P0 == NULL) {
			P0 = pnl_vect_create(sousJacentsSize);
			for (int j = 0; j < sousJacentsSize; j++)
			{
				mmonaiejs = sousJacents[j].monaie;
				double test1 = pnl_mat_get(paths, j, 0);
				double test2 = pnl_mat_get(paths, sousJacentsSize + mmonaiejs, 0);
				pnl_vect_set(P0,j, test1 / test2);
			}
		}
		double prime = GuaranteedRefund;
		bool continu = true;
		int tmp = 0;
		//double test2 = 0.;
		for (int i = 0; i < QuarterlyObservationDates->size; i++)
		{
			tmp = (*InversedDates)[(int)pnl_vect_int_get(QuarterlyObservationDates, i)];
			for (int j = 0; j < sousJacentsSize; j++)
			{
				//test = pnl_mat_get(paths, j, tmp);
				mmonaiejs = sousJacents[j].monaie;
				if (pnl_mat_get(paths, j, tmp) / pnl_mat_get(paths, sousJacentsSize + mmonaiejs, tmp) < MinimalPercentage*GET(P0, j))
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

		for (int i = 0; i < sousJacentsSize; i++)
		{
			S0 = GET(P0, i);
			mmonaiejs = sousJacents[i].monaie;
			for (int j = 0; j < AnnualObservationDates->size; j++)
			{
				//test = (int)pnl_vect_int_get(AnnualObservationDates, j);
				tmp = (*InversedDates)[(int)pnl_vect_int_get(AnnualObservationDates, j)];
				//test2 = (pnl_mat_get(paths, i, tmp) - S0) / S0;
				sum += (pnl_mat_get(paths, i, tmp) / pnl_mat_get(paths, sousJacentsSize + mmonaiejs, tmp) - S0) / S0;
			}
		}
		if (sum > 0)
		{
			prime += Turnout * 100 * sum / (paths->m * AnnualObservationDates->size);
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
		int countCurrency = 0;
		SousJacent sj;
		map<curency, int> CurencyMap;

		for (int i = 0; i < sousJacentsSize; i++) {
			sj = sousJacents[i];

			if (CurencyMap.count(sj.monaie) == 0 && sj.monaie != EUR) {
				CurencyMap[sj.monaie] = countCurrency;
				countCurrency++;
			}
		}
		return (int)CurencyMap.size();
	}

	int Prisma::getDateIterateur(int date)
	{
		return (*InversedDates)[date];
	}

	Prisma::Prisma(){
		DividendAmont = 6.;
		GuaranteedRefund = 70.;
		Turnout = 0.2;
		FixedPremium = 8.;
		date  start = date(3, 10, 2002, Jeudi);
		date  end = date(27, 12, 2007);


		Dates = pnl_vect_int_create(start.getNbDay(end));
		TimeManager::fillOpenDates(Dates, start, end);
		_T = pnl_vect_int_get(Dates, Dates->size - 1);
		int index = 0;
		size_t sizeDate = Dates->size;
		InversedDates; //= new map<int, int>();
		for (int i = 0; i < sizeDate; i++) {
			index = pnl_vect_int_get(Dates, i);
			(*InversedDates)[index] = i;
		}


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

		QuarterlyObservationDates = pnl_vect_int_create(1);
		AnnualObservationDates = pnl_vect_int_create(1);
		DividendesDates = pnl_vect_int_create(1);
		TimeManager::pnlVect_From_Date_List(QuarterlyObservationDates,start, quarterlyDateslist);
		TimeManager::pnlVect_From_Date_List(AnnualObservationDates,start, annualDateslist);
		TimeManager::pnlVect_From_Date_List(DividendesDates,start, dividendesDateslist);


		sousJacentsSize = 16;

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

		//SousJacent sousjacent;
		char name[] = "LVMH";
		/*sousjacent.monaie = EUR;
		sousjacent.setName (name);
		sousJacents[0]=sousjacent;*/
		sousJacents[0] = SousJacent(name, EUR);

		
		char name1[] = "TELECOM ITALIA";
		/*sousjacent.monaie = EUR;
		sousjacent.setName(name1);
		sousJacents[1] = sousjacent;*/
		sousJacents[1] = SousJacent(name1, EUR);

		
		char  name2[] = "NOKIA";
		/*sousjacent.monaie = SEK;
		sousjacent.setName(name2);
		sousJacents[2] = sousjacent;*/
		sousJacents[2] = SousJacent(name2, SEK);

		
		char  name3[] = "DEUTSCHE BANK";
		/*sousjacent.monaie = EUR;
		sousjacent.setName(name3);
		sousJacents[3] = sousjacent;*/
		sousJacents[3] = SousJacent(name3, EUR);

		
		char  name4[] = "AEGON";
		/*sousjacent.monaie = EUR;
		sousjacent.setName(name4);
		sousJacents[4] = sousjacent;*/
		sousJacents[4] = SousJacent(name4, EUR);

		
		char  name5[] = "SCHLUMBERGER";
		/*sousjacent.monaie = USD;
		sousjacent.setName(name5);
		sousJacents[5] = sousjacent;*/
		sousJacents[5] = SousJacent(name5, USD);

		
		char  name6[] = "NTTDoCoMo";
		/*sousjacent.monaie = JPY;
		sousjacent.setName(name6);
		sousJacents[6] = sousjacent;*/
		sousJacents[6] = SousJacent(name6, JPY);

		
		char  name7[] = "HONDA MOTOR";
		/*sousjacent.monaie = JPY;
		sousjacent.setName(name7);
		sousJacents[7] = sousjacent;*/
		sousJacents[7] = SousJacent(name7, JPY);

		
		char  name8[] = "TOSHIBA";
		/*sousjacent.setName(name8);
		sousJacents[8] = sousjacent;
		sousjacent.monaie = JPY;*/
		sousJacents[8] = SousJacent(name8, JPY);

		
		char  name9[] = "PROCTER & GAMBLE";
		/*sousjacent.monaie = USD;
		sousjacent.setName(name9);
		sousJacents[9] = sousjacent;*/
		sousJacents[9] = SousJacent(name9, USD);

		
		char  name10[] = "BANK OF AMERICA";
		/*sousjacent.monaie = USD;
		sousjacent.setName(name10);
		sousJacents[10] = sousjacent;*/
		sousJacents[10] = SousJacent(name10, USD);

		
		char  name11[] = "ENEL";
		/*sousjacent.monaie = EUR;
		sousjacent.setName(name11);
		sousJacents[11] = sousjacent;*/
		sousJacents[11] = SousJacent(name11, EUR);

		
		char  name12[] = "LEGAL & GENERAL";
		/*sousjacent.monaie = GBP;
		sousjacent.setName(name12);
		sousJacents[12] = sousjacent;*/
		sousJacents[12] = SousJacent(name12, GBP);

		
		char  name13[] = "BRIDGESTONE";
		/*sousjacent.monaie = JPY;
		sousjacent.setName(name13);
		sousJacents[13] = sousjacent;*/
		sousJacents[13] = SousJacent(name13, JPY);

		
		char  name14[] = "AIR LQUIDE";
		/*sousjacent.monaie = EUR;
		sousjacent.setName(name14);
		sousJacents[14] = sousjacent;*/
		sousJacents[14] = SousJacent(name14, EUR);

		
		char  name15[] = "GENERALI";
		/*sousjacent.monaie = EUR;
		sousjacent.setName(name15);
		sousJacents[15] = sousjacent;*/
		sousJacents[15] = SousJacent(name15, EUR);
	}
}