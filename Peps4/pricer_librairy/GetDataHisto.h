#pragma once

#include "Data.h"
#include "GetData.h"

namespace Outils
{
	class GetDataHisto :public GetData {
	public:
		void LoadData(int day, Data data);
		void LoadAllData(int start, int end, Data data);
	};
}