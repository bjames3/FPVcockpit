/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** servoout_example.pde
** Demonstrate Servo Signal Output functionality
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#include <ServoOut.h>
#include <Timer1.h>

#define SERVOS 4

uint8_t  g_pinsIn[SERVOS] = {A0, A1, A2, A3}; // Input pins
uint8_t  g_pinsOut[SERVOS] = {2, 3, 4, 5};    // Output pins
uint16_t g_input[SERVOS];                     // Input buffer for servoOut, microseconds
uint8_t  g_work[SERVOOUT_WORK_SIZE(SERVOS)];  // we need to have a work buffer for the ServoOut class

// ServoOut requires three buffers:
//     Pins buffer containing output pins
//     Input buffer containing input values in microseconds
//     Work buffer of SERVOOUT_WORK_SIZE(SERVOS) elements for internal calculations
// This setup removes any technical limit on the number of servos you want,
// and makes sure the library doesn't use more memory than it really needs,
// since the client code supplies the buffers.
rc::ServoOut g_ServoOut(g_pinsOut, g_input, g_work, SERVOS);

void setup()
{
	// Initialize timer1
	rc::Timer1::init();
	
	for (uint8_t i = 0;  i < SERVOS; ++i)
	{
		// set up input pins
		pinMode(g_pinsIn[i], INPUT);
		
		// set up output pins
		pinMode(g_pinsOut[i], OUTPUT);
		
		// put them low
		digitalWrite(g_pinsOut[i], LOW);
		
		// fill input buffer, convert raw values to normalized ones
		g_input[i] = map(analogRead(g_pinsIn[i]), 0, 1024, 1000, 2000);
	}
	g_ServoOut.start();
}

void loop()
{
	// update the input buffer
	for (uint8_t i = 0;  i < SERVOS; ++i)
	{
		g_input[i] = map(analogRead(g_pinsIn[i]), 0, 1024, 1000, 2000);
	}
	
	// tell ServoOut there are new values available in the input buffer
	g_ServoOut.update();
}
