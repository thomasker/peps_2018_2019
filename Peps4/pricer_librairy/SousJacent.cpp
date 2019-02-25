#include "stdafx.h"
#include "sousjacent.h"

namespace Produits
{
	SousJacent::SousJacent()
	{
		m_name[0] = 0;
	}
	SousJacent::SousJacent(char * name, curency curencys)
	{
		strcpy_s(m_name, name);
		monaie = curencys;
	}
	void SousJacent::setName(char * name)
	{
		strcpy_s(m_name, name);
	}

	const char * SousJacent::getName() 
	{
		return m_name;
	}

}