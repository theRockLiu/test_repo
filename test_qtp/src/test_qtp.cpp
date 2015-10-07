//============================================================================
// Name        : test_qtp.cpp
// Author      : TheRockLiuHY
// Version     :
// Copyright   : Personal!
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

#include <qtp.h>

class myalgo: public qtp::trade_base
{
public:
	void handle_quot(struct quot_info& qi)
	{

	}
};

int main()
{
	qtp_engine qe;
	qe.init(/*...*/);

	myalgo c;
	uint32_t quots = DCE_L2_QUOT | CTP_L1_QUOT;
	TRY(c.init(quots));
	qe.add_algo(c);

	myalgo1 c1;
	TRY(c1.init(quots));
	qe.add_algo(c1);

	return qe.run();
}
