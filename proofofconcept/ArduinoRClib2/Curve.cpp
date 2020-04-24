/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** Curve.cpp
** Curve functionality
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#include <Curve.h>


namespace rc
{

// Public functions

Curve::Curve()
{
	// set up a linear curve by default
	m_points[0] = -256;
	m_points[1] = -192;
	m_points[2] = -128;
	m_points[3] = -64;
	m_points[4] = 0;
	m_points[5] = 64;
	m_points[6] = 128;
	m_points[7] = 192;
	m_points[8] = 256;
}


void Curve::setPoint(uint8_t p_point, int16_t p_value)
{
	if (p_point < PointCount)
	{
		m_points[p_point] = p_value;
	}
}


int16_t Curve::getPoint(uint8_t p_point) const
{
	if (p_point < PointCount)
	{
		return m_points[p_point];
	}
	return 0;
}


int16_t& Curve::operator[](uint8_t p_index)
{
	return m_points[p_index];
}


const int16_t& Curve::operator[](uint8_t p_index) const
{
	return m_points[p_index];
}


int16_t Curve::apply(int16_t p_value) const
{
	p_value += 256; // range [0 - 512]
	int16_t index = p_value >> 6;   // divide by 64, range [0 - 8]
	int16_t rem   = p_value & 0x3F; // remainder of division
	
	// linear interpolation on curve values
	int16_t lowval = index >= 8 ? m_points[8] : m_points[index];
	++index;
	int16_t highval = index >= 8 ? m_points[8] : m_points[index];
	
	lowval  = lowval * (64 - rem);
	highval = highval * rem;
	
	return (lowval + highval) >> 6; // weighted average
}


// namespace end
}
