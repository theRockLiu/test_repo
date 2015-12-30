/*
 * struct.h
 *
 *  Created on: Nov 5, 2015
 *      Author: rock
 */

#ifndef INCLUDE_DATA_H_
#define INCLUDE_DATA_H_

typedef struct addr_info
{
	std::string ip_;
	uint16_t port_;
} addr_info_t;

#define LLRB_SIZE (1024 * 1024 * 1024)
#define ELEM_SIZE (256)

#endif /* INCLUDE_DATA_H_ */
