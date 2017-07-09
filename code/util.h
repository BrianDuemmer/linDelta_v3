/*
 * util.h
 *
 * provides a few odds and ends utilities
 *
 *  Created on: May 21, 2017
 *      Author: Duemmer
 */

#ifndef CODE_UTIL_H_
#define CODE_UTIL_H_

#include <stdint.h>

uint64_t currTime();		// returns the current system time in usecs
uint64_t usecsToClockCycles(uint64_t usecs); // converts usecs into number of clock cycles

float mapf(float in, float inMin, float inMax, float outMin, float outMax);
int32_t mapi(int32_t in, int32_t inMin, int32_t inMax, int32_t outMin, int32_t outMax);

float constrainf(float in, float min, float max);
int32_t constraini(int32_t in, int32_t min, int32_t max);


#endif /* CODE_UTIL_H_ */
