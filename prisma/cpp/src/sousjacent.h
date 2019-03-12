#pragma once

#include "Curency.h"

namespace Produits
{
	class   SousJacent
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
