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

namespace qtp_fw
{

class engine
{
public:
	typedef std::shared_ptr<qtp_fw::engine> pointer_t;
public:
	engine();
	~engine();

public:
	int_fast32_t run(const string_t& cf);
};

} /* namespace qtp_fw */

#endif /* ENGINE_H_ */
