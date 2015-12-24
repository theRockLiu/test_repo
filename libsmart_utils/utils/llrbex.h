/*
 * llrbex.h
 *
 *  Created on: Dec 22, 2015
 *      Author: rock
 */

#ifndef UTILS_LLRBEX_H_
#define UTILS_LLRBEX_H_

struct smart_llrb
{
		uint64_t in;
		uint64_t out;
		uint64_t mask;
		uint32_t esize;
		void *data;
} __attribute__ ((aligned (64)));



#endif /* UTILS_LLRBEX_H_ */
