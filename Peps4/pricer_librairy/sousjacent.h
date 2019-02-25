#pragma once

#include "ExportDll.h"

#include "Curency.h"

namespace Produits
{
	class  PRICER_API SousJacent
	{
	public:
		SousJacent();
		SousJacent(char *name, curency curencys);
		void setName(char *name);
		const char* getName();
		curency monaie;
	private:
		char m_name[20];
	};
}
