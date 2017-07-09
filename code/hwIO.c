/*
 * hwIO.h
 *
 * Deals with all direct hardware access and control, as well
 * as providing a neat interface for the high level code
 *
 *  Created on: May 21, 2017
 *      Author: Duemmer
 */

#include "code/dat.h"
#include "code/util.h"
#include <driverlib/gpio.h>
#include <inc/hw_memmap.h>
#include <stdint.h>
#include <stdbool.h>
#include "board.h"
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/fpu.h>
#include <driverlib/ssi.h>
#include <driverlib/pwm.h>
#include "driverlib/interrupt.h"
#include "code/hwIO.h"


/**
 * Initializes all of the board peripherals, such as GPIOs, PWMs, etc.
 * This is essentially the global board initialization routine.
 *
 * NOTE: if using initConfig() in dat.h, that should go after the SD init
 * routines and berfore all the other ones
 */
void hwIO_init()
{
	// set the clock up to the max - 120MHz
	uint32_t foo = 0;
	foo =	SysCtlClockFreqSet(SYSCTL_CFG_VCO_480 | SYSCTL_USE_PLL | SYSCTL_XTAL_25MHZ | SYSCTL_OSC_MAIN, 120000000);

	usecsToClockCycles(foo);


	// enable the GPIOs
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOQ);

	// Enable the other peripherals
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI3);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_EMAC0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_EPHY0);

	// Setup the FPU, with lazy stacking
	FPUEnable();
	FPULazyStackingEnable();


	// run the GPIO init routines
	hwIO_init_portA();
	hwIO_init_portB();
	hwIO_init_portD();
	hwIO_init_portE();
	hwIO_init_portF();
	hwIO_init_portG();
	hwIO_init_portH();
	hwIO_init_portK();
	hwIO_init_portL();
	hwIO_init_portM();
	hwIO_init_portN();
	hwIO_init_portP();
	hwIO_init_portQ();

	// run any other init() routines
	//hwIO_init_SD();
	hwIO_init_Thermo();
	hwIO_init_PWM();
}





///////////////////////////////////////////////////////////////////////////
////////////////////////////// GPIO Port Init /////////////////////////////
///////////////////////////////////////////////////////////////////////////


/*
 * CONTENTS:
 * LED1 - pin4 - out
 * Step1.en - pin5 - out
 * GPIO_GEN_5 - pin6 - in
 */
void hwIO_init_portA()
{
	GPIODirModeSet(GPIO_PORTA_BASE, // outputs
			GPIO_PIN_4 |
			GPIO_PIN_5,
			GPIO_DIR_MODE_OUT );

	GPIODirModeSet(GPIO_PORTA_BASE, // inputs
			GPIO_PIN_6,
			GPIO_DIR_MODE_IN );

	GPIOPadConfigSet(GPIO_PORTA_BASE, // setup the outputs
			GPIO_PIN_4 |
			GPIO_PIN_5,
			GPIO_STRENGTH_8MA,
			GPIO_PIN_TYPE_STD);

	GPIOPadConfigSet(GPIO_PORTA_BASE, // setup the inputs
			GPIO_PIN_6,
			GPIO_STRENGTH_8MA,
			GPIO_PIN_TYPE_STD_WPU);

	GPIOIntEnable(GPIO_PORTA_BASE, //enable interrupts
			GPIO_PIN_6);

	GPIOIntTypeSet(GPIO_PORTA_BASE, // set interrupt mode tto both edges
			GPIO_PIN_6,
			GPIO_BOTH_EDGES);
}




/*
 * CONTENTS:
 * GPIO_GEN_1 - pin2 - in
 * GPIO_GEN_2 - pin3 - in
 * SD.CS - pin4 - HW
 * SD.CLK - pin5 - HW
 */
void hwIO_init_portB()
{
	GPIODirModeSet(GPIO_PORTB_BASE, // HW
			GPIO_PIN_4 |
			GPIO_PIN_5,
			GPIO_DIR_MODE_HW );

	GPIODirModeSet(GPIO_PORTB_BASE, // inputs
			GPIO_PIN_2 |
			GPIO_PIN_3,
			GPIO_DIR_MODE_IN );

	GPIOPinTypeSSI(GPIO_PORTB_BASE, // setup the HW pins as SSI, for the SD Card
			GPIO_PIN_4 |
			GPIO_PIN_5);

	// configure the pin config to use the SSI pins for the SD card
	GPIOPinConfigure(GPIO_PB4_SSI1FSS);
	GPIOPinConfigure(GPIO_PB5_SSI1CLK);

	GPIOPadConfigSet(GPIO_PORTB_BASE, // setup the inputs
			GPIO_PIN_2 |
			GPIO_PIN_3,
			GPIO_STRENGTH_8MA,
			GPIO_PIN_TYPE_STD_WPU);

	GPIOIntEnable(GPIO_PORTB_BASE, //enable interrupts
			GPIO_PIN_2 |
			GPIO_PIN_3);

	GPIOIntTypeSet(GPIO_PORTB_BASE, // set interrupt mode to both edges
			GPIO_PIN_2 |
			GPIO_PIN_3,
			GPIO_BOTH_EDGES);

	// bind input interrupts
//	GPIOIntRegister(GPIO_PORTB_BASE, &portB_ISR);
}





/*
 * CONTENTS:
 * AxB.et - pin0 - in
 * AxA.encB - pin1 - in
 * Step1.dir - pin4 - out
 * Step1.step - pin5 - HW
 * AIN_GEN_1 - pin7 - analog
 */
void hwIO_init_portD()
{
	GPIODirModeSet(GPIO_PORTD_BASE, // HW
			GPIO_PIN_5,
			GPIO_DIR_MODE_HW );

	GPIODirModeSet(GPIO_PORTD_BASE, // inputs
			GPIO_PIN_0 |
			GPIO_PIN_1,
			GPIO_DIR_MODE_IN );

	GPIOPinTypeTimer(GPIO_PORTD_BASE, // setup the HW pin as a timer CCP
			GPIO_PIN_5);

	// configure the pin config to use the SSI pins for the SD card
	GPIOPinConfigure(GPIO_PD5_T3CCP1);

	GPIOPadConfigSet(GPIO_PORTD_BASE, // setup the inputs
			GPIO_PIN_0 |
			GPIO_PIN_1,
			GPIO_STRENGTH_4MA,
			GPIO_PIN_TYPE_STD_WPU);

	GPIOPinTypeADC(GPIO_PORTD_BASE, // enable pin 7 as analog input
			GPIO_PIN_7);

	GPIOIntEnable(GPIO_PORTD_BASE, //enable interrupts
			GPIO_PIN_0 |
			GPIO_PIN_1);

	GPIOIntTypeSet(GPIO_PORTD_BASE, // set interrupt mode to both edges
			GPIO_PIN_0 |
			GPIO_PIN_1,
			GPIO_BOTH_EDGES);

	// bind input interrupts
//	GPIOIntRegister(GPIO_PORTD_BASE, &portD_ISR);
}




/**
 * CONTENTS:
 * AIN_GEN_5 - pin0 - analog
 * AIN_GEN_4 - pin1 - analog
 * AIN_GEN_3 - pin2 - analog
 * AIN_GEN_2 - pin3 - analog
 * SD.MOSI - pin4 - HW
 * SD.MISO - pin5 - HW
 */
void hwIO_init_portE()
{
	GPIODirModeSet(GPIO_PORTE_BASE, // HW
			GPIO_PIN_4 |
			GPIO_PIN_5,
			GPIO_DIR_MODE_HW );

	GPIOPinTypeADC(GPIO_PORTE_BASE, // analog
			GPIO_PIN_0 |
			GPIO_PIN_1 |
			GPIO_PIN_2 |
			GPIO_PIN_3);

	GPIOPinTypeSSI(GPIO_PORTE_BASE, // setup the HW pins as SSI, for the SD Card
			GPIO_PIN_4 |
			GPIO_PIN_5);

	// configure the pin config to use the SSI pins for the SD card
	GPIOPinConfigure(GPIO_PE4_SSI1XDAT0);
	GPIOPinConfigure(GPIO_PE5_SSI1XDAT1);
}




/**
 * CONTENTS:
 * AxB.Mot - pin1 - PWM
 * AxA.Mot - pin2 - PWM
 * AxC.Mot - pin3 - PWM
 */
void hwIO_init_portF()
{
	GPIOPinTypePWM(GPIO_PORTF_BASE,
			GPIO_PIN_1 |
			GPIO_PIN_2 |
			GPIO_PIN_3);

	// configure the pin config to use PWM for te pins
	GPIOPinConfigure(GPIO_PF1_M0PWM1);
	GPIOPinConfigure(GPIO_PF2_M0PWM2);
	GPIOPinConfigure(GPIO_PF3_M0PWM3);
}





/**
 * CONTENTS:
 * Hotend1 - pin1 - out
 */
void hwIO_init_portG()
{
	// configure pin 1 as an 8ma output
	GPIODirModeSet(GPIO_PORTG_BASE,
			GPIO_PIN_1,
			GPIO_DIR_MODE_OUT);

	GPIOPadConfigSet(GPIO_PORTG_BASE,
			GPIO_PIN_1,
			GPIO_STRENGTH_8MA,
			GPIO_PIN_TYPE_STD);
}





/*
 * CONTENTS:
 * Step2.MS3 - pin0 - out
 * Step2.MS2 - pin1 - out
 * AxA.eb - pin2 - in
 * AxA.encA - pin 3 - in
 */
void hwIO_init_portH()
{
	GPIODirModeSet(GPIO_PORTH_BASE, // outputs
			GPIO_PIN_0 |
			GPIO_PIN_1,
			GPIO_DIR_MODE_OUT );

	GPIODirModeSet(GPIO_PORTH_BASE, // inputs
			GPIO_PIN_2 |
			GPIO_PIN_3,
			GPIO_DIR_MODE_IN );

	GPIOPadConfigSet(GPIO_PORTH_BASE, // setup the outputs
			GPIO_PIN_0 |
			GPIO_PIN_1,
			GPIO_STRENGTH_8MA,
			GPIO_PIN_TYPE_STD);

	GPIOPadConfigSet(GPIO_PORTH_BASE, // setup the inputs
			GPIO_PIN_2 |
			GPIO_PIN_3,
			GPIO_STRENGTH_4MA,
			GPIO_PIN_TYPE_STD_WPU);

	GPIOIntEnable(GPIO_PORTH_BASE, //enable interrupts
			GPIO_PIN_2 |
			GPIO_PIN_3);

	GPIOIntTypeSet(GPIO_PORTH_BASE, // set interrupt mode to both edges
			GPIO_PIN_2 |
			GPIO_PIN_3,
			GPIO_BOTH_EDGES);

	// bind input interrupts
//	GPIOIntRegister(GPIO_PORTH_BASE, &portH_ISR);
}






/*
 * CONTENTS:
 * Thermist1 - pin0 - analog
 * Thermist2 - pin1 - analog
 * LED3 - pin2 - out
 * LED2 - pin3 - out
 * Hotend2 - pin4 - out
 * Bed - pin5 - out
 * Step2.MS1 - pin6 - out
 * Step2.en - pin7 - out
 */
void hwIO_init_portK()
{
	GPIODirModeSet(GPIO_PORTK_BASE, // outputs
			GPIO_PIN_2 |
			GPIO_PIN_3 |
			GPIO_PIN_4 |
			GPIO_PIN_5 |
			GPIO_PIN_6 |
			GPIO_PIN_7,
			GPIO_DIR_MODE_OUT );


	GPIOPadConfigSet(GPIO_PORTK_BASE, // setup the outputs
			GPIO_PIN_2 |
			GPIO_PIN_3 |
			GPIO_PIN_4 |
			GPIO_PIN_5 |
			GPIO_PIN_6 |
			GPIO_PIN_7,
			GPIO_STRENGTH_8MA,
			GPIO_PIN_TYPE_STD);

	GPIOPinTypeADC(GPIO_PORTK_BASE, // setup the analog inputs
			GPIO_PIN_0 |
			GPIO_PIN_1);
}





/*
 * CONTENTS:
 * AxC.encA - pin0 - in
 * AxC.encB - pin1 - in
 * AxC.eb - pin2 - in
 * AxC.et - pin3 - in
 * proxSensor - pin4 - in
 */
void hwIO_init_portL()
{
	GPIODirModeSet(GPIO_PORTL_BASE, // inputs
			GPIO_PIN_0 |
			GPIO_PIN_1 |
			GPIO_PIN_2 |
			GPIO_PIN_3 |
			GPIO_PIN_4,
			GPIO_DIR_MODE_IN );


	GPIOPadConfigSet(GPIO_PORTL_BASE, // setup the inputs
			GPIO_PIN_0 |
			GPIO_PIN_1 |
			GPIO_PIN_2 |
			GPIO_PIN_3 |
			GPIO_PIN_4,
			GPIO_STRENGTH_8MA,
			GPIO_PIN_TYPE_STD_WPU);

	// bind input interrupts
//	GPIOIntRegister(GPIO_PORTL_BASE, &portL_ISR);
}






/*
 * CONTENTS:
 * Step2.dir - pin1 - out
 * Step2.step - pin2 - TIMER PWM
 * AxA.et - pin3 - in
 * GPIO_GEN_4 - pin4 - in
 * GPIO_GEN_3 - pin5 - in
 */
void hwIO_init_portM()
{
	GPIODirModeSet(GPIO_PORTM_BASE, // input
			GPIO_PIN_3 |
			GPIO_PIN_4 |
			GPIO_PIN_5,
			GPIO_DIR_MODE_IN );

	GPIOPadConfigSet(GPIO_PORTM_BASE, // setup the input
			GPIO_PIN_3 |
			GPIO_PIN_4 |
			GPIO_PIN_5,
			GPIO_STRENGTH_8MA,
			GPIO_PIN_TYPE_STD_WPU);


	GPIODirModeSet(GPIO_PORTM_BASE, // output
			GPIO_PIN_1,
			GPIO_DIR_MODE_OUT );

	GPIOPadConfigSet(GPIO_PORTM_BASE, // setup the output
			GPIO_PIN_1,
			GPIO_STRENGTH_8MA,
			GPIO_PIN_TYPE_STD);


	GPIOPinTypeTimer(GPIO_PORTM_BASE, // setup the timer PWM pin
			GPIO_PIN_2);

	GPIOPinConfigure(GPIO_PM2_T3CCP0);

	// bind input interrupts
//	GPIOIntRegister(GPIO_PORTM_BASE, &portM_ISR);
}






/*
 * CONTENTS:
 * AxB.eb - pin2 - in
 * AxB.encA - pin3 - in
 * Step1.MS1 - pin4 - out
 * Step1.MS2 - pin5 - out
 */
void hwIO_init_portN()
{
	GPIODirModeSet(GPIO_PORTN_BASE, // input
			GPIO_PIN_2 |
			GPIO_PIN_3,
			GPIO_DIR_MODE_IN );

	GPIOPadConfigSet(GPIO_PORTN_BASE, // setup the input
			GPIO_PIN_2 |
			GPIO_PIN_3,
			GPIO_STRENGTH_8MA,
			GPIO_PIN_TYPE_STD_WPU);


	GPIODirModeSet(GPIO_PORTN_BASE, // output
			GPIO_PIN_4 |
			GPIO_PIN_5,
			GPIO_DIR_MODE_OUT );

	GPIOPadConfigSet(GPIO_PORTN_BASE, // setup the output
			GPIO_PIN_4 |
			GPIO_PIN_5,
			GPIO_STRENGTH_8MA,
			GPIO_PIN_TYPE_STD);

	// bind input interrupts
//	GPIOIntRegister(GPIO_PORTN_BASE, &portN_ISR);
}




/*
 * CONTENTS:
 * AxB.encB - pin2 - in
 * Thermo1.sel - pin3 - out
 * Step1.MS3 - pin4 - out
 */
void hwIO_init_portP()
{
	GPIODirModeSet(GPIO_PORTP_BASE, // input
			GPIO_PIN_2,
			GPIO_DIR_MODE_IN );

	GPIOPadConfigSet(GPIO_PORTP_BASE, // setup the input
			GPIO_PIN_2,
			GPIO_STRENGTH_8MA,
			GPIO_PIN_TYPE_STD_WPU);


	GPIODirModeSet(GPIO_PORTP_BASE, // output
			GPIO_PIN_3 |
			GPIO_PIN_4,
			GPIO_DIR_MODE_OUT );

	GPIOPadConfigSet(GPIO_PORTP_BASE, // setup the output
			GPIO_PIN_3 |
			GPIO_PIN_4,
			GPIO_STRENGTH_8MA,
			GPIO_PIN_TYPE_STD);

	// bind input interrupts
//	GPIOIntRegister(GPIO_PORTP_BASE, &portP_ISR);
}





/*
 * CONTENTS:
 * Thermo.CLK - pin0 - HW
 * Thermo2.sel - pin1 - out
 * Thermo.DAT - pin3 - HW
 */
void hwIO_init_portQ()
{
	GPIODirModeSet(GPIO_PORTQ_BASE, // output
			GPIO_PIN_1,
			GPIO_DIR_MODE_OUT );

	GPIOPadConfigSet(GPIO_PORTN_BASE, // setup the output
			GPIO_PIN_1,
			GPIO_STRENGTH_8MA,
			GPIO_PIN_TYPE_STD);


	// setup the SSI pins
	GPIOPinTypeSSI(GPIO_PORTQ_BASE,
			GPIO_PIN_0 |
			GPIO_PIN_3);

	GPIOPinConfigure(GPIO_PQ0_SSI3CLK);
	GPIOPinConfigure(GPIO_PQ3_SSI3XDAT1);
}




/////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// Other INITs ///////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

/**
 * Initialized the thermocouple bank. Clock speeds and other parameters are set
 * in dat.h
 */
void hwIO_init_Thermo()
{
	SSIConfigSetExpClk(SSI3_BASE,
			SysCtlClockGet(),
			thermo_comMode,
			SSI_MODE_MASTER,
			thermo_clk,
			thermo_bitsPerFrame);

	// turn off both modules to start off
	GPIOPinWrite(GPIO_PORTP_BASE, GPIO_PIN_3, GPIO_PIN_3);
	GPIOPinWrite(GPIO_PORTQ_BASE, GPIO_PIN_1, GPIO_PIN_1);

//	SSIEnable(SSI3_BASE);
}





/**
 * Initializes the PWM Outputs
 */
void hwIO_init_PWM()
{
	uint32_t periodTicks = usecsToClockCycles(axisADat.mot.period);

	// setup generators 0 and 1
	PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_SYNC);
	PWMGenConfigure(PWM0_BASE, PWM_GEN_1, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_SYNC);

	//set the periods
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, periodTicks);
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, periodTicks);

	// set to a safe starting value
	writeMotA(0);
	writeMotB(0);
	writeMotC(0);
}






/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////// raw ISRs ////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////


void portA_ISR() // handles and directs all interrupts on port A
{
	// read the masked interrupt status
	uint32_t intStat = GPIOIntStatus(GPIO_PORTA_BASE, true);

	GPIOIntClear(GPIO_PORTA_BASE, 0xff); // clear interrupt status

	//see which interrupts have fired, and run an corresponding handlers
	if(intStat & GPIO_PIN_6) { gpio_gen5_ISR(); }  // GPIO_GEN_5
}



void portB_ISR() // handles and directs all interrupts on port B
{
	// read the masked interrupt status
	uint32_t intStat = GPIOIntStatus(GPIO_PORTB_BASE, true);

	GPIOIntClear(GPIO_PORTB_BASE, 0xff); // clear interrupt status

	//see which interrupts have fired, and run an corresponding handlers
	if(intStat & GPIO_PIN_2) { gpio_gen1_ISR(); }  // GPIO_GEN_1
	if(intStat & GPIO_PIN_3) { gpio_gen2_ISR(); }  // GPIO_GEN_1
}



void portD_ISR() // handles and directs all interrupts on port D
{
	// read the masked interrupt status
	uint32_t intStat = GPIOIntStatus(GPIO_PORTD_BASE, true);

	GPIOIntClear(GPIO_PORTD_BASE, 0xff); // clear interrupt status

	//see which interrupts have fired, and run an corresponding handlers
	if(intStat & GPIO_PIN_0) { axisB_et_ISR(); }  // top endstop of axis B
	if(intStat & GPIO_PIN_3) { axisA_enc_ISR(); }  // encoder A
}



void portH_ISR() // handles and directs all interrupts on port H
{
	// read the masked interrupt status
	uint32_t intStat = GPIOIntStatus(GPIO_PORTH_BASE, true);

	GPIOIntClear(GPIO_PORTH_BASE, 0xff); // clear interrupt status

	//see which interrupts have fired, and run an corresponding handlers
	if(intStat & GPIO_PIN_2) { axisA_eb_ISR(); }  // bottom endstop of axis A
	if(intStat & GPIO_PIN_3) { axisA_enc_ISR(); }  // encoder A
}




void portL_ISR() // handles and directs all interrupts on port L
{
	// read the masked interrupt status
	uint32_t intStat = GPIOIntStatus(GPIO_PORTL_BASE, true);

	GPIOIntClear(GPIO_PORTL_BASE, 0xff); // clear interrupt status

	//see which interrupts have fired, and run an corresponding handlers
	if(intStat & (GPIO_PIN_0 || GPIO_PIN_1)) { axisC_enc_ISR(); }  // encoder C
	if(intStat & GPIO_PIN_2) { axisC_eb_ISR(); }  // bottom endstop of axis C
	if(intStat & GPIO_PIN_3) { axisC_et_ISR(); }  // top endstop of axis C
	if(intStat & GPIO_PIN_4) { proxSensor_ISR(); }  // proximity sensor
}



void portM_ISR() // handles and directs all interrupts on port M
{
	// read the masked interrupt status
	uint32_t intStat = GPIOIntStatus(GPIO_PORTM_BASE, true);

	GPIOIntClear(GPIO_PORTM_BASE, 0xff); // clear interrupt status

	//see which interrupts have fired, and run an corresponding handlers
	if(intStat & GPIO_PIN_3) { axisA_et_ISR(); }  // top endstop of axis A
	if(intStat & GPIO_PIN_4) { gpio_gen4_ISR(); }  // GPIO_GEN_4
	if(intStat & GPIO_PIN_5) { gpio_gen3_ISR(); }  // GPIO_GEN_3
}





void portN_ISR() // handles and directs all interrupts on port N
{
	// read the masked interrupt status
	uint32_t intStat = GPIOIntStatus(GPIO_PORTN_BASE, true);

	GPIOIntClear(GPIO_PORTN_BASE, 0xff); // clear interrupt status

	//see which interrupts have fired, and run an corresponding handlers
	if(intStat & GPIO_PIN_2) { axisB_eb_ISR(); }  // bottom endstop of axis B
	if(intStat & GPIO_PIN_3) { axisB_enc_ISR(); }  // Axis B encoder
}




void portP_ISR() // handles and directs all interrupts on port P
{
	// read the masked interrupt status
	uint32_t intStat = GPIOIntStatus(GPIO_PORTP_BASE, true);

	GPIOIntClear(GPIO_PORTP_BASE, 0xff); // clear interrupt status

	//see which interrupts have fired, and run an corresponding handlers
	if(intStat & GPIO_PIN_2) { axisB_enc_ISR(); }  // Axis B encoder
}




//////////////// helper functions /////////////////


/**
 * Converts quadrature state to phase position, based on the
 * following:
 *
 * !A,!B = 0
 * A,!B = 1
 * A,B = 2
 * A,!B = 3
 *
 */
uint8_t encQuadToState(bool pinA, bool pinB)
{
	uint8_t phase;

	if(!pinA && !pinB) { phase = 0; }
	if(pinA && !pinB)  { phase = 1; }
	if(pinA && pinB)   { phase = 2; }
	if(!pinA && pinB)  { phase = 3; }

	return phase;
}



/////////////////// Specific ISRs ///////////////////

// General GPIO ISRs. Change the implementation of these as needed
void gpio_gen1_ISR();
void gpio_gen2_ISR();
void gpio_gen3_ISR();
void gpio_gen4_ISR();
void gpio_gen5_ISR();


/** Encoder ISRs
 *
 *  These run whenever either pin updates. They read both pins and update the encoder accordingly
 */

void axisA_enc_ISR()
{
	static uint8_t prev = 0;

	// read pin states
	bool pinA = GPIOPinRead(GPIO_PORTH_BASE, GPIO_PIN_3);
	bool pinB = GPIOPinRead(GPIO_PORTD_BASE, GPIO_PIN_1);

	// convert to phase
	uint8_t curr = encQuadToState(pinA, pinB);

	//update encoder counts
	if(curr > prev || (curr == 0 && prev == 4)) // should increment
	{
		encA_cts++;
	} else if(curr != prev) // should decrement
	{
		encA_cts--;
	} else {} // illegal state. this would require interrupts to either be picked up multiple times or missed entirely

	prev = curr;
}



// TODO implement all of these
void axisA_et_ISR() {}
void axisA_eb_ISR() {}

void axisB_enc_ISR() {}
void axisB_et_ISR() {}
void axisB_eb_ISR() {}

void axisC_enc_ISR() {}
void axisC_et_ISR() {}
void axisC_eb_ISR() {}

void proxSensor_ISR() {}

void gpio_gen1_ISR() {}
void gpio_gen2_ISR() {}
void gpio_gen3_ISR() {}
void gpio_gen4_ISR() {}
void gpio_gen5_ISR() {}







///////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// Access Functions ///////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////


/**
 * querys one of the thermocouple modules for its temperature
 *
 * @param isModule1 if true, querys module 1. Else, querys module 2
 */
float getThermoTemp(bool isModule1)
{
	 // pull thermo1 select low if isModule1 is true, high otherwise
	 // pull thermo2 select low if isModule1 is false, high otherwise
	if(isModule1)
	{
		GPIOPinWrite(GPIO_PORTP_BASE, GPIO_PIN_3, 0);
		GPIOPinWrite(GPIO_PORTQ_BASE, GPIO_PIN_1, 0xff);
	} else
	{
		GPIOPinWrite(GPIO_PORTP_BASE, GPIO_PIN_3, 0xff);
		GPIOPinWrite(GPIO_PORTQ_BASE, GPIO_PIN_1, 0);
	}

	// read from the thermocouple
	uint32_t rawDat = 0;
	SSIEnable(SSI3_BASE);
	SSIDataPutNonBlocking(SSI3_BASE, 64);
	SSIDataGet(SSI3_BASE, &rawDat);
	SSIDisable(SSI3_BASE);

	// turn off both modules
	GPIOPinWrite(GPIO_PORTP_BASE, GPIO_PIN_3, GPIO_PIN_3);
	GPIOPinWrite(GPIO_PORTQ_BASE, GPIO_PIN_1, GPIO_PIN_1);

	// extract temperature information
	rawDat >>= 1;
	rawDat &= 0xfff;
	float actualTemp = rawDat * thermo_tempScl;

	return actualTemp;
}





void writeMotA(float output)
{
	// constrain the output to be on [-1, 1]
	constrainf(output, -1, 1);

	// adjust for deadband
	uint32_t adjLow = axisADat.mot.low + axisADat.mot.deadband;
	uint32_t adjHigh = axisADat.mot.high - axisADat.mot.deadband;

	uint32_t usecsHigh = mapf(output, -1, 1, adjLow, adjHigh); // convert from output % to usecs high

	// add back deadband time if output != 0
	if(output > 0) { usecsHigh += axisADat.mot.deadband; }
	else if(output < 0) { usecsHigh -= axisADat.mot.deadband; }

	//convert to clock cycles
	uint32_t ticksHigh = usecsToClockCycles(usecsHigh);

	//write that value to the PWM output
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, ticksHigh);
}




void writeMotB(float output)
{
	// constrain the output to be on [-1, 1]
	constrainf(output, -1, 1);

	// adjust for deadband
	uint32_t adjLow = axisBDat.mot.low + axisBDat.mot.deadband;
	uint32_t adjHigh = axisBDat.mot.high - axisBDat.mot.deadband;

	uint32_t usecsHigh = mapf(output, -1, 1, adjLow, adjHigh); // convert from output % to usecs high

	// add back deadband time if output != 0
	if(output > 0) { usecsHigh += axisBDat.mot.deadband; }
	else if(output < 0) { usecsHigh -= axisBDat.mot.deadband; }

	//convert to clock cycles
	uint32_t ticksHigh = usecsToClockCycles(usecsHigh);

	//write that value to the PWM output
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, ticksHigh);
}




void writeMotC(float output)
{
	// constrain the output to be on [-1, 1]
	constrainf(output, -1, 1);

	// adjust for deadband
	uint32_t adjLow = axisCDat.mot.low + axisCDat.mot.deadband;
	uint32_t adjHigh = axisCDat.mot.high - axisCDat.mot.deadband;

	uint32_t usecsHigh = mapf(output, -1, 1, adjLow, adjHigh); // convert from output % to usecs high

	// add back deadband time if output != 0
	if(output > 0) { usecsHigh += axisCDat.mot.deadband; }
	else if(output < 0) { usecsHigh -= axisCDat.mot.deadband; }

	//convert to clock cycles
	uint32_t ticksHigh = usecsToClockCycles(usecsHigh);

	//write that value to the PWM output
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_3, ticksHigh);
}




void setMotorsEnabled(bool enable)
{
	if(enable) { PWMGenEnable(PWM0_BASE, PWM_GEN_0 | PWM_GEN_1); }
	else { PWMGenDisable(PWM0_BASE, PWM_GEN_0 | PWM_GEN_1); }

	PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT | PWM_OUT_2_BIT | PWM_OUT_3_BIT, enable);
}





float getEncAPos()
{
	float ret = ((float) encA_cts) / axisADat.enc.ppi; // convert counts to distance
	if(axisADat.enc.inv) { ret *= -1; } // invert if necessary

	return ret;
}



void setStatusLEDs(bool b1, bool b2, bool b3)
{
	GPIOPinWrite(GPIO_PORTA_BASE,
			GPIO_PIN_4,
			b1 << 4);

	GPIOPinWrite(GPIO_PORTK_BASE,
			GPIO_PIN_3,
			b2 << 3);

	GPIOPinWrite(GPIO_PORTK_BASE,
			GPIO_PIN_2,
			b3 << 2);

}















