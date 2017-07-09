/*
 * hwIO.h
 *
 * Deals with all direct hardware access and control, as well
 * as providing a neat interface for the high level code
 *
 *  Created on: May 21, 2017
 *      Author: Duemmer
 */

#ifndef CODE_HWIO_H_
#define CODE_HWIO_H_

#include <stdint.h>
#include <stdbool.h>

// useful fields

// current counts on the encoders
int32_t encA_cts;
int32_t encB_cts;
int32_t encC_cts;

// encoder counts from the last call of hwIO_update()
int32_t encA_cts_prev;
int32_t encB_cts_prev;
int32_t encC_cts_prev;


// Raw GPIO ISRs
void portA_ISR();
void portB_ISR();
void portD_ISR();
void portH_ISR();
void portL_ISR();
void portM_ISR();
void portN_ISR();
void portP_ISR();


// functions for specific pin interrupts
void axisA_enc_ISR();
void axisA_et_ISR();
void axisA_eb_ISR();

void axisB_enc_ISR();
void axisB_et_ISR();
void axisB_eb_ISR();

void axisC_enc_ISR();
void axisC_et_ISR();
void axisC_eb_ISR();

void proxSensor_ISR();

void gpio_gen1_ISR();
void gpio_gen2_ISR();
void gpio_gen3_ISR();
void gpio_gen4_ISR();
void gpio_gen5_ISR();

//helper functions, used in intermediate processing
uint8_t encQuadToState(bool pinA, bool pinB); // converts from quadrature to phase position on the encoders


// utility functions to provide a clean interface for high level code
float getEncAPos();
float getEncAVel();
void setEncA(uint32_t newPos); // sets the current position of encoder A

bool getAxA_et(); //fetches the current state of the top endstop on axis A
bool getAxA_eb(); //fetches the current state of the bottom endstop on axis A

float getEncBPos();
float getEncBVel();
void setEncB(uint32_t newPos); // sets the current position of encoder B

bool getAxB_et(); //fetches the current state of the top endstop on axis A
bool getAxB_eb(); //fetches the current state of the bottom endstop on axis A

float getEncCPos();
float getEncCVel();
void setEncC(uint32_t newPos); // sets the current position of encoder C

bool getAxC_et(); //fetches the current state of the top endstop on axis A
bool getAxC_eb(); //fetches the current state of the bottom endstop on axis A

bool getProxSensor(); // gets the current state of the proximity sensor

float getThermoTemp(bool isModule1); // reads the temperature of thermocouple module

void setMotorsEnabled(bool enable); // turn on and off the pwm generator

void writeMotA(float output); // writes the output to axis A's motor
void writeMotB(float output); // writes the output to axis B's motor
void writeMotC(float output); // writes the output to axis C's motor

void setStatusLEDs(bool b1, bool b2, bool b3); // sets the states of the 3 status LEDs


void hwIO_update(); // run once per tick, updates all time domain hwIO stuff
void hwIO_init(); // initializes all pin modes, as well as any relevant peripherals

// GPIO port setups
void hwIO_init_portA();
void hwIO_init_portB();
void hwIO_init_portD();
void hwIO_init_portE();
void hwIO_init_portF();
void hwIO_init_portG();
void hwIO_init_portH();
void hwIO_init_portK();
void hwIO_init_portL();
void hwIO_init_portM();
void hwIO_init_portN();
void hwIO_init_portP();
void hwIO_init_portQ();

void hwIO_init_SD();	// initialized the SD card communications
void hwIO_init_Thermo();// initializes the thermocouple bank
void hwIO_init_PWM();


#endif /* CODE_HWIO_H_ */











