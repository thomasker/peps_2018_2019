#include "pch.h"

#include <iostream>
#include "TestDates.h"
#include <map>

namespace Test
{
	TestDates::TestDates()
	{
	}

	TestDates::~TestDates()
	{
	}

	bool TestDates::TestTotal()
	{
		std::cout << "===== START TEST TOTAL DATE =====\n";
		bool isOK = true;
		if (!TestAddDay()) {
			std::cout << "Test Add Day---------- Not OK!\n";
			isOK = false;
		}
		else
		{
			std::cout << "Test Add Day OK!\n";
		}

		if (!TestIsWeekend()) {
			std::cout << "Test Is Weekend---------- Not OK!\n";
			isOK = false;
		}
		else
		{
			std::cout << "Test Is Weekend OK!\n";
		}

		if (!TestIsClosedDay()) {
			std::cout << "Test Is Closed Day---------- Not OK!\n";
			isOK = false;
		}
		else
		{
			std::cout << "Test Is Closed Day OK!\n";
		}

		if (!TestGetNbDay()) {
			std::cout << "Test Get Nb Day---------- Not OK!\n";
			isOK = false;
		}
		else
		{
			std::cout << "Test Get Nb Day OK!\n";
		}

		if (!TestAreEqual()) {
			std::cout << "Test Are Equal---------- Not OK!\n";
			isOK = false;
		}
		else
		{
			std::cout << "Test Are Equal OK!\n";
		}

		if (!TestOperator()) {
			std::cout << "Test Operator---------- Not OK!\n";
			isOK = false;
		}
		else
		{
			std::cout << "Test Operator OK!\n";
		}
		return  isOK;
	}

	bool TestDates::TestAddDay()
	{
		// changement de jour
		date a = date(1, 1, 2002, Lundi);
		a.addDay();
		if (!(a == date(2, 1, 2002, Mardi))) {
			std::cout << "error add day basic !\n";
			return false;
		}

		//changement de mois
		a = date(31, 1, 2002, Mardi);
		a.addDay();
		if (!(a == date(1, 2, 2002, Mercredi))) {
			std::cout << "error add day mois !\n";
			return false;
		}

		//changement d' année
		a = date(31, 12, 2002, Mercredi);
		a.addDay();
		if (!(a == date(1, 1, 2003, Jeudi))) {
			std::cout << "error add day annee !\n";
			return false;
		}
		//anné non bixextile
		a = date(28, 2, 2002, Jeudi);
		a.addDay();
		if (!(a == date(1, 3, 2002, Vendredi))) {
			std::cout << "error add day non bixextile !\n";
			return false;
		}
		//anné bixextile
		a = date(28, 2, 2004, Vendredi);
		a.addDay();
		if (!(a == date(29, 2, 2004, Samedi))) {
			std::cout << "error add day bixextile !\n";
			return false;
		}
		a.addDay();
		if (!(a == date(1, 3, 2004, Dimanche))) {
			std::cout << "error add day samedi > dimanche  !\n";
			return false;
		}
		a.addDay();
		if (!(a == date(2, 3, 2004, Lundi))) {
			std::cout << "error add day dimanche >lundi  !\n";
			return false;
		}
		return true;
	}

	bool TestDates::TestIsWeekend()
	{
		date a = date(1, 1, 2002, Lundi);
		if (a.isWeekend()) {
			std::cout << "error is Weekend Lundi !\n";
			return false;
		}
		a = date(1, 1, 2002, Mardi);
		if (a.isWeekend()) {
			std::cout << "error is Weekend Mardi !\n";
			return false;
		}
		a = date(1, 1, 2002, Mercredi);
		if (a.isWeekend()) {
			std::cout << "error is Weekend Mercredi !\n";
			return false;
		}
		a = date(1, 1, 2002, Jeudi);
		if (a.isWeekend()) {
			std::cout << "error is Weekend Jeudi !\n";
			return false;
		}
		a = date(1, 1, 2002, Vendredi);
		if (a.isWeekend()) {
			std::cout << "error is Weekend Vendredi !\n";
			return false;
		}
		a = date(1, 1, 2002, Samedi);
		if (!a.isWeekend()) {
			std::cout << "error is Weekend Samedi !\n";
			return false;
		}
		a = date(1, 1, 2002, Dimanche);
		if (!a.isWeekend()) {
			std::cout << "error is Weekend Dimanche !\n";
			return false;
		}
		return true;
	}

	bool TestDates::TestIsClosedDay()
	{
		date a = date(1, 1, 2002);
		if (!a.isClosedDay()) {
			std::cout << "error Is Closed Day 1 !\n";
			return false;
		}
		a = date(31, 12, 2007);
		if (!a.isClosedDay()) {
			std::cout << "error Is Closed Day 2 !\n";
			return false;
		}
		a = date(2, 2, 2001);
		if (a.isClosedDay()) {
			std::cout << "error Is Closed Day 3 !\n";
			return false;
		}
		return true;
	}

	bool TestDates::TestGetNbDay()
	{
		date a = date(1, 1, 2002);
		if (a.getNbDay(date(2, 1, 2002)) != 1) {
			std::cout << "error Get Nb Day 1!\n";
			return false;
		}
		a = date(1, 1, 2002);
		if (a.getNbDay(date(1, 2, 2002)) != 31) {
			std::cout << "error Get Nb Day 31!\n";
			return false;
		}
		a = date(1, 1, 2002);
		if (a.getNbDay(date(1, 1, 2004)) != 365 * 2) {
			std::cout << "error Get Nb Day 2 ans!\n";
			return false;
		}
		return true;
	}

	bool TestDates::TestAreEqual()
	{
		date a = date(1, 1, 2002);
		if (!a.areEqual(date(5, 5, 2002), date(5, 5, 2002))) {
			std::cout << "error are equal faux a la place de vrai !\n";
			return false;
		}
		a = date(1, 1, 2002);
		if (a.areEqual(date(4, 5, 2002), date(5, 5, 2002))) {
			std::cout << "error are equal vrais a la place de faux  !\n";
			return false;
		}
		return true;
	}

	bool TestDates::TestOperator()
	{
		date a = date(1, 1, 2002);
		if (!(a == date(1, 1, 2002))) {
			std::cout << "error == equal !\n";
			return false;
		}
		a = date(5, 3, 2002);
		if (!(a < date(6, 3, 2002) ||
			a < date(4, 4, 2002) ||
			a < date(4, 2, 2005))
			) {
			std::cout << "error < inf !\n";
			return false;
		}
		a = date(5, 3, 2002);
		if (!(a > date(4, 3, 2002) ||
			a > date(6, 2, 2002) ||
			a > date(6, 8, 2001))
			) {
			std::cout << "error < inf !\n";
			return false;
		}
		return true;
	}

}