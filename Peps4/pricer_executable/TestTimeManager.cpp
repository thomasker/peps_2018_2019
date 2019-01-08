#include "pch.h"
#include "TestTimeManager.h"

namespace Test
{
	bool TestTimeManager::TestTotal()
	{
		std::cout << "===== START TEST TOTAL TIME MANAGER =====\n";
		bool isOK = true;
		if (!TestPnlVect_From_Date_List()) {
			std::cout << "Test PnlVect From Date List---------- Not OK!\n";
			isOK = false;
		}
		else
		{
			std::cout << "Test PnlVect From Date List OK!\n";
		}
		if (!TestfillOpenDates()) {
			std::cout << "Test fill Open Dates---------- Not OK!\n";
			isOK = false;
		}
		else
		{
			std::cout << "Test fill Open Dates OK!\n";
		}
		return isOK;
	}

	bool TestTimeManager::TestPnlVect_From_Date_List()
	{
		list<date> dateList = { date(1,1,2002) };
		PnlVectInt* result = TimeManager::pnlVect_From_Date_List(date(1, 1, 2002), dateList);
		if (result->size != 1 || pnl_vect_int_get(result, 0) != 0) {
			std::cout << "error PnlVect From Date List 1 date = start !\n";
			return false;
		}

		dateList = { date(5,1,2002), date(3,1,2002) };
		result = TimeManager::pnlVect_From_Date_List(date(1, 1, 2002), dateList);
		if (result->size != 2 || pnl_vect_int_get(result, 0) != 4 || pnl_vect_int_get(result, 1) != 2) {
			std::cout << "error PnlVect From Date List 2 date non trillee !\n";
			return false;
		}

		dateList = { date(3,1,2002) , date(5,1,2002) };
		result = TimeManager::pnlVect_From_Date_List(date(1, 1, 2002), dateList);
		if (result->size != 2 || pnl_vect_int_get(result, 0) != 2 || pnl_vect_int_get(result, 1) != 4) {
			std::cout << "error PnlVect From Date List 2 date  trillee !\n";
			return false;
		}

		return true;
	}

	bool TestTimeManager::TestfillOpenDates()
	{
		PnlVectInt* vect = pnl_vect_int_create(1);
		TimeManager::fillOpenDates(vect, date(1, 2, 2002, Lundi), date(1, 2, 2002, Lundi));
		if (vect->size != 1) {
			std::cout << "error fill OpenDates start = end !\n";
			return false;
		}

		TimeManager::fillOpenDates(vect, date(1, 2, 2002, Lundi), date(7, 2, 2002, Dimanche));
		if (vect->size != 5) {
			std::cout << "error fill OpenDates one week!\n";
			return false;
		}

		TimeManager::fillOpenDates(vect, date(1, 2, 2002, Lundi), date(21, 2, 2002, Dimanche));
		if (vect->size != 15) {
			std::cout << "error fill OpenDates three weeks!\n";
			return false;
		}
		if (pnl_vect_int_get(vect, 5) != 7.0) {
			std::cout << "error fill OpenDates three weeks!\n";
			return false;
		}


		TimeManager::fillOpenDates(vect, date(28, 12, 2002, Vendredi), date(4, 1, 2003, Vendredi));
		if (vect->size != 4) {
			std::cout << "error fill OpenDates closed Days!\n";
			return false;
		}
		return true;
	}

}