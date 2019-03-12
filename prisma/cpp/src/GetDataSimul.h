#pragma once

#include "Data.h"
#include "GetData.h"
#include "pnl/pnl_random.h"

namespace Outils
{
	class GetDataSimul :public GetData {
	public:
		void LoadData(int day, Data data);
		void LoadAllData(int start, int end, Data data);
	private:
		PnlRng* _rng;

		double _r;
		double _thetha;
		double _S0;
		int _T;
	};
}