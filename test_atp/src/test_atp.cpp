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

using namespace atp;

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


class myalgo: public atp::algo_base
{
public:
	void handle_quot(quot_info_t& qi)
	{

	}

	void handle_cmd_res(trade_cmd_res_t& cr)
	{
		if (ATP_ORDER_CMD == cr.type)
		{
			//price_vec[cr.pl].

		}
		else if (ATP_CANCEL_CMD == cr.type)
		{

		}
		else
		{
			SU_ASSERT(false);
		}
	}

	void handle_match_res(match_res_t &)
	{

	}

private:
	std::vector<pli_t> price_vec;
};

int main()
{
	atp::algo_engine ae;
	ae.init();

	atp::algo_base::pointer_t p = make_shared<myalgo>();
	std::vector<std::string> vec = {"test"};
	ae.reg_algo(p, vec);
	return ae.run_and_wait();
}

