/*
 * main.c
 *
 *  Created on: Sep 16, 2015
 *      Author: rock
 */

#include <stdint.h>

struct X {

	char c;
	int x;

};

struct Y {

	char c;
	int x;
};//__attribute__((packed));

int32_t main(int32_t argc, char* argv[]) {

	struct X x;
	struct Y y = {.c = argc + 1, .x = argc + 2};

	x.c = y.c + argc;
	x.x = y.x - argc;

	return x.c + x.x + y.c + y.x;

}

////[rock@localhost Release]$ objdump -d -Shl -M intel test_struct > test_struct1.s

