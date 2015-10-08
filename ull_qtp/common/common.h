/*
 * common.h
 *
 *  Created on: Sep 16, 2015
 *      Author: rock
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <string>
#include <cassert>

typedef std::string string_t;

enum
{
	RET_SUC = 0
};

#define ASSERT(expr) assert(expr)

#endif /* COMMON_H_ */
