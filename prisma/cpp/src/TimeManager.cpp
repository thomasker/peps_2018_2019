#include "TimeManager.h"
#include <sstream>
#include <iostream>
#include <fstream>

namespace Outils
{
	void TimeManager::pnlVect_From_Date_List(PnlVectInt* dateVect,const date  startDate, list<date> dateList) {
		pnl_vect_int_resize(dateVect,(int)dateList.size());
		date dateActuel = startDate;
		int count = 0;
		list<date>::iterator it = dateList.begin();
		for (int i = 0; i < (int)dateList.size(); i++) {
			if (dateActuel > *it) {
				dateActuel = startDate;
				count = 0;
			}
			count += dateActuel.getNbDay(*it);
			pnl_vect_int_set(dateVect, i, count);
			dateActuel = *it;
			it++;
		}
	}

	void TimeManager::fillOpenDates(PnlVectInt* dates, date  start, date  end) {
		pnl_vect_int_resize(dates, start.getNbDay(end)+1);
		date  date = start;
		int nbDay = 0;
		int i = 0;
		if (!date.isWeekend() && !date.isClosedDay())
		{
			pnl_vect_int_set(dates, i, nbDay);
			i++;
		}
		while (date < end) {
			date.addDay();
			nbDay++;
			if (!date.isWeekend() && !date.isClosedDay())
			{
				pnl_vect_int_set(dates, i, nbDay);
				i++;
			}
		}
		pnl_vect_int_resize(dates, i);
	}

	string TimeManager::GetDateInFormat(date start, int nbDay)
	{
		date result = date(start);
		for (int i = 0; i < nbDay; i++) {
			result.addDay();
		}
		std::ostringstream strs;
		strs << result._anne << "-" << result._mois << "-" << result._jour;
		return strs.str();
	}

}
