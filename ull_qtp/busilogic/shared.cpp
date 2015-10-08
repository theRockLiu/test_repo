/*
 * shared.cpp
 *
 *  Created on: Sep 16, 2015
 *      Author: rock
 */

#include "shared.h"

namespace qtp
{

shared::shared()
{
	// TODO Auto-generated constructor stub

}

shared::~shared()
{
	// TODO Auto-generated destructor stub
}

void shared::handle_quot(struct quot_info& qi)
{
	///TODO: quant logic.

	///send command
	struct order_info oi = {0};
	int_fast32_t ret = async_send_command(oi);
}

int_fast32_t shared::async_send_command(struct order_info& oi)
{

}

} /* namespace qtp_bl */

void qtp::shared::exec()
{
	///check trade conn
	dth_.check_conn();

	///check quot conn
	dqh_.check_conn();
}

void qtp::shared::handle_x()
{
}

void qtp::shared::handle_res()
{
}

//int_fast32_t qtp::shared::async_send_command(struct order_info& oi)
//{
//}
