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

int32_t main(int32_t argc, char* argv[])
{
	if (2 != argc)
	{
		return -1;
	}

	string_t cf("default.cfg");

	qtp_fw::engine::pointer_t peg = std::make_shared<qtp_fw::engine>();

	return peg->run(cf);
}
