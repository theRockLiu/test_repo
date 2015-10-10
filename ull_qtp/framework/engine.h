/*
 * engine.h
 *
 *  Created on: Sep 16, 2015
 *      Author: rock
 */

#ifndef ENGINE_H_
#define ENGINE_H_

#include <cstdint>
#include <string>
#include <memory>

#include "../common/common.h"

namespace qtp
{




class qtp_engine
{
public:
	typedef std::shared_ptr<qtp::qtp_engine> pointer_t;
public:
	qtp_engine();
	~qtp_engine();

public:
	int_fast32_t add_algo();
	int_fast32_t run(const string_t& cf);
};

} /* namespace qtp */

#endif /* ENGINE_H_ */
