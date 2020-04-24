/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** channel_example.pde
** Demonstrate Channel functionality
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#include <Channel.h>


// we define a single channel here, for the sake of simplicity
rc::Channel g_channel;

void setup()
{
	// say our servo is moving in the wrong direction, we'd want to use channel reverse
	g_channel.setReverse(true);
	
	// and for some reason we want to limit the throw of our servo
	g_channel.setEndPointMin(80);
	
	// and our servo's center might be a tad bit off
	g_channel.setSubtrim(20);
}

void loop()
{
	// we use A0 as input pin
	int16_t normalized = map(analogRead(A0), 0, 1024, -256, 256);
	
	// and apply channel transformations
	normalized = g_channel.apply(normalized);
	
	// we can then use the transformed value for further modification
	// or we can transmit it using the PPMOut class
}
