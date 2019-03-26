#include "Hello.h"
#include <iostream>
#include "Pricing.h"
using namespace Pricing;
void sayHelloWorld() { std::cout << "Hello World!" << std::endl; }

int returnInteger() { return 42; }

char * jsonprisma()
{
	std::string str = PricingOptions::hedgePrisma();
	char *cstr = new char[str.length() + 1];
	strcpy(cstr, str.c_str());
	return cstr;
}

//string jsonPrismaString()
//{
//	return string();
//}
