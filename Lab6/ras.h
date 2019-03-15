/*
 * ras.h
 *
 *  Created on: march 11, 2019
 *
 *  created by Alex Dai and Hamsa Waseem
 */

#include <stdint.h>
#include <stdbool.h>

#ifndef RAS_H_
#define RAS_H_

void rasInit();

void rasRead(uint32_t data[]);

#endif /* ras_H_ */
