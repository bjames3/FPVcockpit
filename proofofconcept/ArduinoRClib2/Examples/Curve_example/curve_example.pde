/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** curve_example.pde
** Demonstrate Curve functionality
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#include <Curve.h>


rc::Curve g_curve;

void setup()
{
	// by default we have a linear curve from -256 to 256
	// for this example we create a V shaped curve, for stunt mode throttle on a heli
	g_curve[0] = 256;
	g_curve[1] = 192;
	g_curve[2] = 128;
	g_curve[3] = 64;
}

void loop()
{
	// we use A0 as input pin
	int16_t normalized = map(analogRead(A0), 0, 1024, -256, 256);
	
	// and apply the curve
	normalized = g_curve.apply(normalized);
	
	// we can then use the transformed value for further modification
	// or we can transmit it using the PPMOut class
}
