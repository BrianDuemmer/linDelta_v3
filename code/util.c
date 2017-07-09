/*
 * util.c
 *
 *  Created on: May 26, 2017
 *      Author: Duemmer
 */

#include "code/util.h"
#include <stdint.h>
#include <stdbool.h>
#include <driverlib/sysctl.h>

uint64_t usecsToClockCycles(uint64_t usecs)
{
	return (usecs * (uint64_t)SysCtlClockGet()) / 1000000000;
}




float mapf(float in, float inMin, float inMax, float outMin, float outMax)
{
	return (in - inMin) * (outMax - outMin)/(inMax - inMin) + outMin;
}



int32_t mapi(int32_t in, int32_t inMin, int32_t inMax, int32_t outMin, int32_t outMax)
{
	return (in - inMin) * (outMax - outMin)/(inMax - inMin) + outMin;
}




float constrainf(float in, float min, float max)
{
	if(in < min) {in = min; }
	else if(in > max) {in = max; }

	return in;
}

int32_t constraini(int32_t in, int32_t min, int32_t max)
{
	if(in < min) {in = min; }
	else if(in > max) {in = max; }

	return in;
}















