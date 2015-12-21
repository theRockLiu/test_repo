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


//class myalgo: public satp::algo_base
//{
//public:
//	void handle_quot(quot_info_t& qi)
//	{
//
//	}
//
//	void handle_cmd_res(trade_cmd_res_t& cr)
//	{
//		if (ATP_ORDER_CMD == cr.type)
//		{
//			//price_vec[cr.pl].
//
//		}
//		else if (ATP_CANCEL_CMD == cr.type)
//		{
//
//		}
//		else
//		{
//			SU_CHECK(false);
//		}
//	}
//
//	void handle_match_res(match_res_t &)
//	{
//
//	}
//
//private:
//	std::vector<pli_t> price_vec;
//};

int main()
{
	bool flag = true;

	satp::algo_trade_platform ap;
	ap.init();

	satp::exc_info_t x;
	satp::quot_engine::pointer_t qe = ap.create_quot_engine(x);
	satp::trade_engine::pointer_t te = ap.create_trade_engine(x);

	ap.start();
	while(flag)
	{
		evt_t *evt = (evt_t*)qe->get_data();
		///TODO: .....
		evt_t *evt1 = (evt_t*)te->get_data();
		///TODO:.....
		///te.async_send_cmd(...);
	}
	ap.stop();

}

