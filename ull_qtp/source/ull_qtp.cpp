//============================================================================
// Name        : ull_qtp.cpp
// Author      : TheRockLiuHY
// Version     :
// Copyright   : Personal!
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

#include "../framework/engine.h"

class algo1 : public atp::algo_base
{

};


int32_t main(int32_t argc, char* argv[])
{
	if (2 != argc)
	{
		return -1;
	}

	string_t cf("qtp.json");
	atp::engine::pointer_t egn = std::make_shared<atp::engine>();

	return egn->run(cf);
}
