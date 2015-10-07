/*
 * tradehandler.h
 *
 *  Created on: Sep 16, 2015
 *      Author: rock
 */

#ifndef TRADEHANDLER_H_
#define TRADEHANDLER_H_

#include "../apis/dce/Linux/trade/lib/TradeAPI.h"

namespace qtp
{

class dce_trade_handler
{
public:
	dce_trade_handler();
	virtual ~dce_trade_handler();

public:
	void check_conn();

};

} /* namespace qtp_bl */

#endif /* TRADEHANDLER_H_ */
