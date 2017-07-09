/*
 * dat.h
 *
 * Contains all important data variables controlled from the config file.
 * All configuration datastructures are defined here
 *
 *  Created on: May 21, 2017
 *      Author: Duemmer
 */

#ifndef CODE_DAT_H_
#define CODE_DAT_H_

#include <stdint.h>
#include <stdbool.h>
#include <driverlib/ssi.h>

typedef struct EncDat
{
	float ppi; // pulses per inch of axis travel
	bool inv; // if true, multiplies the encoder readout by -1
} EncDat;



typedef struct MotDat
{
	uint32_t period; 	// period (in uSecs) of the PWM
	uint32_t low;		// pulse length for full reverse
	uint32_t high;		// pulse length for full forward
	uint32_t deadband;	// minimum offset from center that will produce an output
	bool inv;			// if true multiplies output by -1
} MotDat;



typedef struct EndstDat
{
	float zPos;		// z-coordinate that the endstop is hit at
	bool inv;		// the pinstate is XOR'd with this to see if it is hit
} EndstDat;



typedef struct PIDDat
{
	float kp;
	float ki;
	float kd;
} PIDDat;



typedef struct AxisDat
{
	EncDat enc;
	MotDat mot;
	PIDDat pid;

	EndstDat et;	// top endstop
	EndstDat eb;	// bottom endstop

	float axisX;	// X-coord of the carriage mount
	float axisY;	// Y-coord of the carriage mount
} AxisDat;







// general information
uint64_t tickTime = 15000; // number of usecs between each time a system update tick occurs


// Axis data
AxisDat axisADat =
{
	.enc = { .ppi = 200, .inv = false },
	.mot = { .period = 7500, .low = 1000, .high = 2000, .deadband = 200, .inv = false },
	.pid = { .kp = 0, .ki = 0, .kd = 0 },
	.et = { .zPos = 21, .inv = true },
	.eb = { .zPos = 2, .inv = true }, .axisX = 10, .axisY = 12
};


AxisDat axisBDat =
{
	.enc = { .ppi = 200, .inv = false },
	.mot = { .period = 7500, .low = 1000, .high = 2000, .deadband = 200, .inv = false },
	.pid = { .kp = 0, .ki = 0, .kd = 0 },
	.et = { .zPos = 21, .inv = true },
	.eb = { .zPos = 2, .inv = true }, .axisX = 10, .axisY = 12
};


AxisDat axisCDat =
{
	.enc = { .ppi = 200, .inv = false },
	.mot = { .period = 7500, .low = 1000, .high = 2000, .deadband = 200, .inv = false },
	.pid = { .kp = 0, .ki = 0, .kd = 0 },
	.et = { .zPos = 21, .inv = true },
	.eb = { .zPos = 2, .inv = true }, .axisX = 10, .axisY = 12
};




// Thermocouple module data
uint32_t thermo_clk = 100000;
uint32_t thermo_bitsPerFrame = 16;
uint32_t thermo_comMode = SSI_FRF_MOTO_MODE_3;
float thermo_tempScl = 0.25;

void initConfig(); // loads everything from a configuration file. Probably won't get implemented till the very end!





#endif /* CODE_DAT_H_ */








