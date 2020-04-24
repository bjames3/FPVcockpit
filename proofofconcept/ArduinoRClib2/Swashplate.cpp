/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** Swashplate.cpp
** Swashplate functionality
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#include <Swashplate.h>


namespace rc
{

// Public functions

Swashplate::Swashplate()
:
m_type(Type_H1),
m_ailMix(0),
m_eleMix(0),
m_pitMix(0)
{
	
}


void Swashplate::setType(Type p_type)
{
	m_type = p_type;
}


Swashplate::Type Swashplate::getType() const
{
	return m_type;
}


void Swashplate::setAilMix(int8_t p_mix)
{
	m_ailMix = p_mix;
}


int8_t Swashplate::getAilMix() const
{
	return m_ailMix;
}


void Swashplate::setEleMix(int8_t p_mix)
{
	m_eleMix = p_mix;
}


int8_t Swashplate::getEleMix() const
{
	return m_eleMix;
}


void Swashplate::setPitMix(int8_t p_mix)
{
	m_pitMix = p_mix;
}


int8_t Swashplate::getPitMix() const
{
	return m_pitMix;
}


void Swashplate::apply(int16_t p_ail,
                       int16_t p_ele,
                       int16_t p_pit,
                       int16_t& p_ailOUT,
                       int16_t& p_eleOUT,
                       int16_t& p_pitOUT,
                       int16_t& p_ele2OUT) const
{
	// input is in [-358 - 358] range, so we risk overflows
	// also keep in mind that mix may be negative
	bool ailneg = p_ail < 0;
	uint16_t ail =  static_cast<uint16_t>(ailneg ? -p_ail : p_ail);
	ailneg ^= m_ailMix < 0;
	ail = (ail * static_cast<uint16_t>(m_ailMix > 0 ? m_ailMix : -m_ailMix)) / 100;
	p_ail = ailneg ? -static_cast<int16_t>(ail) : static_cast<int16_t>(ail);
	
	bool eleneg = p_ele < 0;
	uint16_t ele =  static_cast<uint16_t>(eleneg ? -p_ele : p_ele);
	eleneg ^= m_eleMix < 0;
	ele = (ele * static_cast<uint16_t>(m_eleMix > 0 ? m_eleMix : -m_eleMix)) / 100;
	p_ele = eleneg ? -static_cast<int16_t>(ele) : static_cast<int16_t>(ele);
	
	bool pitneg = p_pit < 0;
	uint16_t pit =  static_cast<uint16_t>(pitneg ? -p_pit : p_pit);
	pitneg ^= m_pitMix < 0;
	pit = (pit * static_cast<uint16_t>(m_pitMix > 0 ? m_pitMix : -m_pitMix)) / 100;
	p_pit = pitneg ? -static_cast<int16_t>(pit) : static_cast<int16_t>(pit);
	
	switch (m_type)
	{
		case Type_H1:
		default:
		{
			p_ailOUT = p_ail;
			p_eleOUT = p_ele;
			p_pitOUT = p_pit;
		}
		break;
		
		case Type_H2:
		{
			p_eleOUT = p_ele;
			p_ailOUT = p_ail + p_pit;
			p_pitOUT = -p_ail + p_pit;
		}
		break;
		
		case Type_HE3:
		{
			p_eleOUT = p_ele + p_pit;
			p_ailOUT = p_ail + p_pit;
			p_pitOUT = -p_ail + p_pit;
		}
		break;
		
		case Type_HR3:
		{
			p_eleOUT = p_ele + p_pit;
			p_ailOUT = p_ail + p_pit -(p_ele >> 1);
			p_pitOUT = -p_ail + p_pit -(p_ele >> 1);
		}
		break;
		
		case Type_HN3:
		{
			p_eleOUT = p_ele + p_pit -(p_ail >> 1);
			p_ailOUT = p_ail + p_pit;
			p_pitOUT = -p_ele + p_pit -(p_ail >> 1);
		}
		break;
		
		case Type_H3:
		{
			p_eleOUT = p_ele + p_pit;
			p_ailOUT = -p_ele + p_ail + p_pit;
			p_pitOUT = -p_ele - p_ail + p_pit;
		}
		break;
		
		case Type_H4:
		{
			p_eleOUT  = p_ele + p_pit;
			p_ele2OUT = -p_ele + p_pit;
			p_ailOUT  = p_ail + p_pit;
			p_pitOUT  = -p_ail + p_pit;
		}
		break;
		
		case Type_H4X:
		{
			p_eleOUT  = (p_ele >> 1) - (p_ail >> 1) + p_pit;
			p_ele2OUT = -(p_ele >> 1) + (p_ail >> 1) + p_pit;
			p_ailOUT  = (p_ele >> 1) + (p_ail >> 1) + p_pit;
			p_pitOUT  = -(p_ele >> 1) - (p_ail >> 1) + p_pit;
		}
		break;
	}
}


// namespace end
}
