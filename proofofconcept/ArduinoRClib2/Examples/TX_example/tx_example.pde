/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** tx_example.pde
** Complete transmitter example.
** This example demonstrates how to make a 6 channel CCPM heli transmitter
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#include <AIPin.h>
#include <Channel.h>
#include <Curve.h>
#include <DIPin.h>
#include <DualRates.h>
#include <Expo.h>
#include <Gyro.h>
#include <PPMOut.h>
#include <Swashplate.h>
#include <Timer1.h>
#include <util.h>

enum
{
	ChannelCount = 6
};

// input related variables
rc::AIPin g_aPins[4] = { rc::AIPin(A0), rc::AIPin(A1), rc::AIPin(A2), rc::AIPin(A3) };
rc::DIPin g_dPins[2] = { rc::DIPin(3), rc::DIPin(4) };

// Expo/DR, we use one expo and one dr per control and per flightmode
rc::Expo g_ailExpo[2] = {rc::Expo(-30), rc::Expo(-10)};
rc::Expo g_eleExpo[2] = {rc::Expo(-30), rc::Expo(-10)};
rc::Expo g_rudExpo[2] = {rc::Expo(-20), rc::Expo(0)};

rc::DualRates g_ailDR[2] = {rc::DualRates(80), rc::DualRates(100)};
rc::DualRates g_eleDR[2] = {rc::DualRates(80), rc::DualRates(100)};
rc::DualRates g_rudDR[2] = {rc::DualRates(80), rc::DualRates(100)};

// throttle and pitch curves
rc::Curve g_thrCurve[2];
rc::Curve g_pitCurve[2];

// throttle hold
rc::Curve g_pitCurveHold  = rc::Curve();
int16_t   g_throttleHoldThrottle = -256;

// channel transformations
rc::Channel g_channels[ChannelCount];

// swashplate
rc::Swashplate g_swash;

// gyro
rc::Gyro g_gyro[2];

// Channel values in microseconds
uint16_t g_channelValues[ChannelCount] = {0};

// PPM related variables
uint8_t    g_PPMWork[PPMOUT_WORK_SIZE(ChannelCount)];
rc::PPMOut g_PPMOut(ChannelCount, g_channelValues, g_PPMWork, ChannelCount);


void setup()
{
	// Initialize timer1
	rc::Timer1::init();
	
	// set default values, these depend on hardware configurations
	g_aPins[0].setCalibration( 90, 515,  930); // Right horizontal, aileron
	g_aPins[1].setCalibration(127, 544,  961); // Right vertical, elevator
	g_aPins[2].setCalibration( 27, 435,  834); // Left vertical, throttle
	g_aPins[3].setCalibration( 67, 502,  924); // Left horizontal, rudder
	g_aPins[3].setReverse(true);               // potentiometer mounted upside down
	
	// throttle curves
	g_thrCurve[1][0] = 256;
	g_thrCurve[1][1] = 192;
	g_thrCurve[1][2] = 128;
	g_thrCurve[1][3] = 64;
	
	// pitch curves
	g_pitCurve[0][0] = 0;
	g_pitCurve[0][1] = 32;
	g_pitCurve[0][2] = 64;
	g_pitCurve[0][3] = 96;
	g_pitCurve[0][4] = 128;
	g_pitCurve[0][5] = 160;
	g_pitCurve[0][6] = 192;
	g_pitCurve[0][7] = 224;
	g_pitCurve[0][8] = 256;
	
	// swashplate settings
	g_swash.setType(rc::Swashplate::Type_HR3);
	g_swash.setAilMix(50);
	g_swash.setEleMix(50);
	g_swash.setPitMix(50);
	
	// gyro settings
	g_gyro[0].setType(rc::Gyro::Type_AVCS);
	g_gyro[1].setType(rc::Gyro::Type_AVCS);
	
	g_gyro[0].setMode(rc::Gyro::Mode_Normal);
	g_gyro[1].setMode(rc::Gyro::Mode_AVCS);
	
	g_gyro[0] = 50;
	g_gyro[1] = 75;
	
	// set up normalized -> microseconds conversion
	rc::setCenter(1520);
	rc::setTravel(600);
	
	// fill channel values buffer with sane values, all centered
	g_channelValues[0] = rc::normalizedToMicros(0);
	g_channelValues[1] = rc::normalizedToMicros(0);
	g_channelValues[2] = rc::normalizedToMicros(-256); // Throttle channel, MUST BE AT 0 THROTTLE!
	g_channelValues[3] = rc::normalizedToMicros(0);
	g_channelValues[4] = rc::normalizedToMicros(0);
	g_channelValues[5] = rc::normalizedToMicros(0);
	
	// set up PPM
	g_PPMOut.setPulseLength(448);
	g_PPMOut.setPauseLength(10448);
	g_PPMOut.start(true);
}


void loop()
{
	// read digital values
	uint8_t flightmode   = g_dPins[0].read();
	uint8_t throttleHold = g_dPins[1].read();
		
	// read analog values
	int16_t AIL = g_aPins[0].read(); // aileron
	int16_t ELE = g_aPins[1].read(); // elevator
	int16_t THR = g_aPins[2].read(); // throttle/pitch
	int16_t RUD = g_aPins[3].read(); // rudder
	
	// apply expo and dual rates to input
	AIL = g_ailExpo[flightmode].apply(AIL);
	AIL = g_ailDR[flightmode].apply(AIL);
	
	ELE = g_eleExpo[flightmode].apply(ELE);
	ELE = g_eleDR[flightmode].apply(ELE);
	
	RUD = g_rudExpo[flightmode].apply(RUD);
	RUD = g_rudDR[flightmode].apply(RUD);
	
	int16_t PIT = THR; // same stick
	
	// apply pitch and throttle curves and handle throttle hold
	if (throttleHold)
	{
		THR = g_throttleHoldThrottle;
		PIT = g_pitCurveHold.apply(PIT);
	}
	else
	{
		THR = g_thrCurve[flightmode].apply(THR);
		PIT = g_pitCurve[flightmode].apply(PIT);
	}
	
	int16_t dummy; // we need a dummy value
	
	// apply swashplate mixing
	g_swash.apply(AIL, ELE, PIT, AIL, ELE, PIT, dummy);
	
	// handle gyro
	int16_t GYR = g_gyro[flightmode].apply();
	
	// perform channel transformations and set channel values
	g_channelValues[0] = rc::normalizedToMicros(g_channels[0].apply(AIL));
	g_channelValues[1] = rc::normalizedToMicros(g_channels[1].apply(ELE));
	g_channelValues[2] = rc::normalizedToMicros(g_channels[2].apply(THR));
	g_channelValues[3] = rc::normalizedToMicros(g_channels[3].apply(RUD));
	g_channelValues[4] = rc::normalizedToMicros(g_channels[4].apply(GYR));
	g_channelValues[5] = rc::normalizedToMicros(g_channels[5].apply(PIT));
	
	// Tell PPMOut that new values are ready
	g_PPMOut.update();
}
