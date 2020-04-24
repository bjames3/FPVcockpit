/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** tx_example.ino
** 
**
** Author: Daniel van den Ouden
** Mods:   William James III
** Project: ArduinoRCLib  (on Sourceforge)
**
**  Cable:
**  Arduino  Pin 9  :  PPM out   to  pin 1(top) FlySky module
**     "     Vin    :  5-12VDC   to  pin 3       ""
**     "     GND    :  GND       to  pin 4       ""
**                                 pin 2,5 No Connect
**         FlySky Channel Order:    AETR
** -------------------------------------------------------------------------*/

#include <AIPin.h>
#include <PPMOut.h>
#include <Timer1.h>
#include <util.h>
#include <Curve.h>
#include <DualRates.h>

//#define Debug

enum
{
	ChannelCount = 6,
        MaxChannels = 8,
        LoRate = 0,
        HiRate = 1
};

// input related variables
rc::AIPin g_aPins[4] = { rc::AIPin(A0), rc::AIPin(A1), rc::AIPin(A2), rc::AIPin(A3) };

//*************************************************************
rc::DualRates g_ailDR[2] = {rc::DualRates(60), rc::DualRates(100)};  
rc::DualRates g_eleDR[2] = {rc::DualRates(60), rc::DualRates(100)};
rc::DualRates g_rudDR[2] = {rc::DualRates(50), rc::DualRates(100)};

rc::Curve g_thrCurve;

//*************************************************************


// Channel values in microseconds
uint16_t g_channelValues[ChannelCount] = {0};

// PPM related variables
uint8_t    g_PPMWork[PPMOUT_WORK_SIZE(MaxChannels)];
rc::PPMOut g_PPMOut(ChannelCount, g_channelValues, g_PPMWork, MaxChannels);


void setup()
{
  
        #ifdef Debug
          Serial.begin(9600);
        #endif
	// Initialize timer1
	rc::Timer1::init();
	
	// set default values, these depend on hardware configurations
	g_aPins[0].setCalibration( 89, 495,  842);  // aileron
	g_aPins[1].setCalibration(208, 502,  853);  // elevator
	g_aPins[2].setCalibration( 125, 454,  783); // throttle   (hi - lo)/2 + lo
	g_aPins[3].setCalibration( 178, 576,  891); // rudder
        //g_aPins[0].setReverse(true);              //reverse aileron
        g_aPins[1].setReverse(true);	            //reverse elevator
        //g_aPins[2].setReverse(true);              //reverse throttle
        //g_aPins[3].setReverse(true);              //reverse rudder
	
	// set up normalized -> microseconds conversion
	rc::setCenter(1500);
	rc::setTravel(600);
	
	// fill channel values buffer with sane values, all centered
	g_channelValues[0] = rc::normalizedToMicros(0);   
	g_channelValues[1] = rc::normalizedToMicros(0);
	g_channelValues[2] = rc::normalizedToMicros(-256);     // Throttle channel, MUST BE AT 0 THROTTLE!
	g_channelValues[3] = rc::normalizedToMicros(0);
        // fill Dummy Channels. These will not change
        g_channelValues[4] = rc::normalizedToMicros(0);
        g_channelValues[5] = rc::normalizedToMicros(0);
        
	// set up PPM
	g_PPMOut.setPulseLength(448);
	g_PPMOut.setPauseLength(10448);
	g_PPMOut.start(true);
}


void loop()
{
	
	// read analog values
	int16_t AIL = g_aPins[0].read(); // aileron
	int16_t ELE = g_aPins[1].read(); // elevator
	int16_t THR = g_aPins[2].read(); // throttle
	int16_t RUD = g_aPins[3].read(); // rudder

//*************************************************
        AIL = g_ailDR[LoRate].apply(AIL);     //hard wired rate, normally selected with switch
        ELE = g_eleDR[LoRate].apply(ELE);
        RUD = g_rudDR[LoRate].apply(RUD);
        
        THR = g_thrCurve.apply(THR);         //uses default curve

//*************************************************
	
	#ifndef Debug
	  //if ( THR < -250 )
          //  THR = -256;
          g_channelValues[0] = rc::normalizedToMicros(AIL);
	  g_channelValues[1] = rc::normalizedToMicros(ELE);
	  g_channelValues[2] = rc::normalizedToMicros(THR);
	  g_channelValues[3] = rc::normalizedToMicros(RUD);
          // dummy values
          g_channelValues[4] = rc::normalizedToMicros(0);
          g_channelValues[5] = rc::normalizedToMicros(0);
        
	  // Tell PPMOut that new values are ready
	  g_PPMOut.update();
        #else
          Serial.print("Throttle: ");
          Serial.println(THR);
          Serial.print("Aileron: ");
          Serial.println(AIL);
          Serial.print("Elevator: ");
          Serial.println(ELE);
          Serial.print("Rudder: ");
          Serial.println(RUD);
          delay(5000);
        #endif
}
