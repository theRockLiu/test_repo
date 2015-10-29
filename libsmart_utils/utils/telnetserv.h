/*
 * telnetserv.h
 *
 *  Created on: Jul 11, 2015
 *      Author: rock
 */

#ifndef UTILS_TELNETSERV_H_
#define UTILS_TELNETSERV_H_

#include <libcli.h>
#include "../base/base.h"

namespace qtp
{

class telnet_serv
{
public:
	telnet_serv();
	virtual ~telnet_serv();

public:
	int_fast32_t run();

private:
    struct cli_def  *cli_;

};

} /* namespace qtp */

#endif /* UTILS_TELNETSERV_H_ */
