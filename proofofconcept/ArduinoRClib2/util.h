#ifndef INC_RC_UTIL_H
#define INC_RC_UTIL_H

/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** util.h
** Utility include file
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#include <inttypes.h>

/*!
 *  \file util.h
 *  \brief Utility include file.
 *  \author    Daniel van den Ouden
 *  \date      Feb-2012
 *  \copyright Public Domain.
*/

namespace rc
{
	/*! \brief convert microseconds to a normalized value [-256 - 256].
	    \param p_micros Input in microseconds.
	    \return Normalized value, range [-256 - 256].*/
	int16_t microsToNormalized(uint16_t p_ticks);
	
	/*! \brief convert a normalized value [-256 - 256] to microseconds.
	    \param p_normal Normalized value, range [-256 - 256].
	    \return Microseconds.*/
	uint16_t normalizedToMicros(int16_t p_normal);
	
	/*! \brief Sets servo center.
	    \param p_center Center of servo in microseconds.*/
	void setCenter(uint16_t);
	
	/*! \brief Gets the servo center.
	    \return The servo center in microseconds.*/
	uint16_t getCenter();
	
	/*! \brief Sets maximum travel from center.
	    \param p_travel Travel of servo in microseconds.*/
	void setTravel(uint16_t);
	
	/*! \brief Gets maximum travel from center.
	    \return Travel of servo in microseconds.*/
	uint16_t getTravel();
	
	/*! \brief Sets timings according to Futaba standards, center 1520, travel 600.*/
	void loadFutaba();
	
	/*! \brief Sets timings according to JR standards, center 1500, travel 600.*/
	void loadJR();
}

#endif // INC_RC_UTIL_H
