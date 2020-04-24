/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** expo_example.pde
** Demonstrate Expo functionality
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#include <Expo.h>


rc::Expo g_expo;

void setup()
{
	// we use 30% expo, dumb down the sensitivity in the center a bit
	g_expo = 30;
}

void loop()
{
	// we use A0 as input pin
	int16_t normalized = map(analogRead(A0), 0, 1024, -256, 256);
	
	// and apply expo
	normalized = g_expo.apply(normalized);
	
	// we can then use the transformed value for further modification
	// or we can transmit it using the PPMOut class
}
