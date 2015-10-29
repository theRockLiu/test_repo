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

#include <utils/defines.h>
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
			ATP_ASSERT(false);
		}
	}

	void handle_match_res()
	{

	}

private:
	std::vector<pli_t> price_vec;
};

int main()
{
	atp::algo_engine ae;
	//ae.init();
	//ae.reg_algo();
	ae.run_and_wait();


//	qtp_engine qe;
//	qe.init(/*...*/);
//
//	myalgo c;
//	uint32_t quots = DCE_L2_QUOT | CTP_L1_QUOT;
//	TRY(c.init(quots));
//	qe.add_algo(c);
//
//	myalgo1 c1;
//	TRY(c1.init(quots));
//	qe.add_algo(c1);
//
//	return qe.run();
}

