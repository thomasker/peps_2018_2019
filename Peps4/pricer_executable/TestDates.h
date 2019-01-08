#pragma once

#include "pch.h"
#include "Date.h"

using namespace Outils;

namespace Test
{
	class TestDates
	{
	public:
		TestDates();
		~TestDates();
		bool TestTotal();
		bool TestAddDay();
		bool TestIsWeekend();
		bool TestIsClosedDay();
		bool TestGetNbDay();
		bool TestAreEqual();
		bool TestOperator();
	private:

	};
}