/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** dualrates_example.pde
** Demonstrate Dual Rates functionality
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#include <DualRates.h>

rc::DualRates g_rates[2];

void setup()
{
	// we set up two rates, one for each flight mode
	g_rates[0] = 80;  // normal mode, 80% response
	g_rates[1] = 100; // stunt mode, we want faster response here
}

void loop()
{
	// we use A0 as input pin
	int16_t normalized = map(analogRead(A0), 0, 1024, -256, 256);
	
	// and apply the rates, we use digital pin 3 as a flight mode switch
	normalized = g_rates[digitalRead(3)].apply(normalized);
	
	// we can then use the transformed value for further modification
	// or we can transmit it using the PPMOut class
}
