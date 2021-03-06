/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** AIPin.cpp
** Analog input read functionality
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#if defined(ARDUINO) && ARDUINO >= 100
	#include <Arduino.h>
#else
	#include <wiring.h>
#endif

#include <AIPin.h>


namespace rc
{

// Public functions

AIPin::AIPin(uint8_t p_pin)
:
m_reversed(false),
m_trim(0),
m_center(511),
m_min(0),
m_max(1023)
{
	setPin(p_pin);
}


void AIPin::setPin(uint8_t p_pin)
{
	m_pin = p_pin;
	pinMode(p_pin, INPUT);
}


uint8_t AIPin::getPin() const
{
	return m_pin;
}


AIPin& AIPin::operator = (uint8_t p_rhs)
{
	setPin(p_rhs);
	return *this;
}


AIPin::operator uint8_t () const
{
	return m_pin;
}


void AIPin::setReverse(bool p_reversed)
{
	m_reversed = p_reversed;
}
	

bool AIPin::isReversed() const
{
	return m_reversed;
}


void AIPin::setTrim(int8_t p_trim)
{
	m_trim = p_trim;
}
	

int8_t AIPin::getTrim() const
{
	return m_trim;
}


void AIPin::setCenter(uint16_t p_center)
{
	m_center = p_center;
}


uint16_t AIPin::getCenter() const
{
	return m_center;
}


void AIPin::setMin(uint16_t p_min)
{
	m_min = p_min;
}


uint16_t AIPin::getMin() const
{
	return m_min;
}


void AIPin::setMax(uint16_t p_max)
{
	m_max = p_max;
}


uint16_t AIPin::getMax() const
{
	return m_max;
}


void AIPin::setCalibration(uint16_t p_min, uint16_t p_center, uint16_t p_max)
{
	setMin(p_min);
	setCenter(p_center);
	setMax(p_max);
}


int16_t AIPin::read() const
{
	uint16_t raw = analogRead(m_pin);

	
	// reverse if needed
	if (m_reversed) raw = 1023 - raw;
	
	// apply trim
	//if (m_reversed)
	//	raw -= m_trim;
	//else
	//    raw += m_trim;
	
	// early abort
	if (raw <= m_min) return -256;
	if (raw >= m_max) return  256;
	
	// calculate distance from center and maximum distance from center
	uint16_t dist = raw > m_center ?   raw - m_center : m_center -   raw;
	uint16_t max = raw > m_center ? m_max - m_center : m_center - m_min;
	
	// change the range from [0 - max] to [0 - 256]
	
	// first bring the value down to below 256, or we'll be getting overflows, we'll compensate later
	int bits = 0;
	while (dist >= 256)
	{
		dist >>= 1;
		++bits;
	}
	
	dist <<= 8;  // multiply by 256
	dist /= max; // bring down to new range
	
	// bring the value back up if we did any truncating before
	while (bits > 0)
	{
		dist <<= 1;
		--bits;
	}
	
	int16_t out = (raw < m_center) ? -static_cast<int16_t>(dist) : static_cast<int16_t>(dist);

	out += m_trim;

	if (out <= -256) return -256;
	if (out >= 256 ) return 256;

	return out;
}


// namespace end
}
