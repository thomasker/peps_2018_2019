#include "stdafx.h"
#include "TimeManager.h"

namespace Outils
{
	PnlVectInt* TimeManager::pnlVect_From_Date_List(const date  startDate, list<date> dateList) {
		PnlVectInt* dateVect = pnl_vect_int_create((int)dateList.size());
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
		return dateVect;
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

}
