#pragma once
#include "pnl/pnl_matvect.h"

namespace Outils
{
	class FileReader
	{
	public:
		FileReader();
		~FileReader();
		void readFile(PnlVect* values, PnlVectInt* dates, char* name);
	private:

	};
}