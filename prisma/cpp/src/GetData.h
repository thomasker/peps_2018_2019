#pragma once

#include "Data.h"

namespace Outils
{
	class GetData {
	public:
		virtual void LoadData(int day , Data data) = 0;
		virtual void LoadAllData(int start, int end, Data data) = 0;
	};
}