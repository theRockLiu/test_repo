//============================================================================
// Name        : test_atp.cpp
// Author      : TheRockLiuHY
// Version     :
// Copyright   : Personal!
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
using namespace std;

#include <base/defines.h>
#include <include/atp.h>

using namespace satp;

typedef struct order_cmd
{
	uint32_t x;
} order_cmd_t;

typedef struct price_level_info
{
	uint32_t bid_posi_c;
	uint32_t bid_posi_nc;
	uint32_t ask_posi_c;
	uint32_t ask_posi_nc;
} pli_t;

//
void do_algo_trade(evt_t *quot_evt, evt_t *trade_evt)
{
	///TODO: run algo trade logic...
}

int main()
{
	///init
	bool flag = true;
	satp::algo_trade_platform ap;
	ap.init();

	///create quot engine and trade engine.
	satp::exc_info_t x;
	std::unordered_map<std::string, uint64_t> &contracts;
	satp::quot_engine::pointer_t qe = ap.create_quot_engine(x, contracts);
	satp::trade_engine::pointer_t te = ap.create_trade_engine(x);

	////now run the engines.
	ap.start();
	while(flag)
	{
		///call algo trade func.
		do_algo_trade(qe->get_evt(), te->get_evt());
	}
	ap.stop();
}

