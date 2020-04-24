/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** ppmin_example.pde
** Demonstrate Pulse Position Modulation Input functionality
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#include <PPMIn.h>
#include <Timer1.h>


#define CHANNELS 4

uint16_t g_values[CHANNELS];                  // output buffer for PPMIn
uint8_t  g_workIn[PPMIN_WORK_SIZE(CHANNELS)]; // we need to have a work buffer for the PPMIn class

rc::PPMIn g_PPMIn(g_values, g_workIn, CHANNELS);

void setup()
{
	// Initialize timer1
	rc::Timer1::init();
	
	// We use pin 8 as PPM input pin
	pinMode(8, INPUT);
	
	// only allow pin change interrupts for PB0, digital pins 8.
	PCMSK0 = (1 << PCINT0);
	
	// enable pin change interrupt 0
	PCICR = (1 << PCIE0);
	
	// start listening
	g_PPMIn.start();
}


void loop()
{
	// update incoming values
	g_PPMIn.update();
	
	if (g_PPMIn.isStable())
	{
		// do magic, incoming values available in g_values in microseconds.
	}
}


// Interrupt handling code below, this needs cleaning

static uint8_t lastB = 0;

// Pin change port 0 interrupt
ISR(PCINT0_vect)
{
	uint8_t newB = PINB;
	
	if ((lastB & 1) != (newB & 1))
	{
		g_PPMIn.pinChanged(newB & 1);
	}
	
	lastB = newB;
}
