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

class my_algo1
{
	public:
		my_algo1()
		{
		}
		~my_algo1()
		{
		}

	public:
		void handle_quot(evt_t* vt)
		{

		}

		void handle_send_order_rsp(evt_t* vt)
		{

		}

		void handle_withdraw_order_rsp(evt_t* vt)
		{

		}
};

typedef void (my_algo1::*my_algo_func)(evt_t*);
struct evt_handler
{
		my_algo_func func_;
		uint8_t evt_id_;
} evt_handlers[] = { { &my_algo1::handle_quot, EVT_QUOT } };

int main()
{
	///init
	satp::algo_trade_platform ap;
	ap.init();
	///create quot engine and trade engine.
	satp::exc_info_t x; // = { "172.56.56.21", 9999 };
	std::vector<std::string> contracts = { "a1603", "a1605" };
	////
	satp::quot_engine::pointer_t qe = ap.create_quot_engine(x, contracts);
	satp::trade_engine::pointer_t te = ap.create_trade_engine(x, contracts);
	///
	my_algo1 ma;
	evt_t *et = NULL;
	////now run the engines.
	ap.start();
	for (;;)
	{
		et = qe->get_evt();
		if (NULL != et)
		{
			(ma.*(evt_handlers[et->id_].func_))(et);
		}

		et = te->get_evt();
		if (NULL != et)
		{
			(ma.*(evt_handlers[et->id_].func_))(et);
		}
	}
	ap.stop();
}

