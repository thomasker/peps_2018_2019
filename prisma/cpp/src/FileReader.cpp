#include <fstream>
#include <iostream>
#include "FileReader.h"
#include "pnl/pnl_matrix.h"
#include "TimeManager.h"
using namespace std;

namespace Outils
{
	FileReader::FileReader()
	{
	}
	FileReader::~FileReader()
	{
	}
	void FileReader::readFile(PnlVect* values, PnlVectInt* dates, char* name) {
		int jour, mois, anne;
		char data[100];
		ifstream infile;
		list <date> DatesList;
		infile.open(name);
		infile >> data;
		char date2 = '\n';
		char* date1 = &date2;
		char** date0= &date1;
		int n = 1000;
		pnl_vect_resize( values ,n);
		double value = 0.;
		int i = 0;
		char** context = new char*();
		char* splitedDate;
		while (infile >> data) {
			char* chars_array = strtok_s(data, ";", date0);
			value = atof(date1);
			if (value == 0. ) {
				splitedDate = strtok_s(data, "/", context);
				jour =(int)atof(splitedDate);
				splitedDate = strtok_s(*context, "/", context);
				mois = (int)atof(splitedDate);
				splitedDate = strtok_s(*context, "/", context);
				anne = (int)atof(splitedDate);
				DatesList.push_back(date(jour, mois, anne));
			}
			else
			{
				if (i >= n) {
					n += 1000;
					pnl_vect_resize(values, n);
				}
				pnl_vect_set(values,i, value);
				i++;
			}
		}
		date firstDate= date(DatesList.front());
		TimeManager::pnlVect_From_Date_List(dates,firstDate, DatesList);
		pnl_vect_resize(values, i);
		//pnl_vect_print(values);
	}
	
}