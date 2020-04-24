#ifndef INC_RC_PPMOUT_H
#define INC_RC_PPMOUT_H

/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** PPMOut.h
** Pulse Position Modulation Output functionality
**
** Author: Daniel van den Ouden
** Project: ArduinoRCLib
** Website: http://sourceforge.net/p/arduinorclib/
** -------------------------------------------------------------------------*/

#include <inttypes.h>

#define PPMOUT_WORK_SIZE(channels) ((channels) + (((channels) + 1) * 4))


namespace rc
{

/*! 
 *  \brief     Class to encapsulate PPM Output functionality.
 *  \details   This class provides a way to generate a PPM signal for a configurable amount of channels.
 *  \author    Daniel van den Ouden
 *  \date      Feb-2012
 *  \copyright Public Domain.
 */
class PPMOut
{
public:
	
	/*! \brief Constructs a PPMOut object.
	    \param p_channels Number of active channels, <= p_maxChannels.
	    \param p_input External input buffer for channel values, in microseconds.
	    \param p_work Work buffer, should be (p_maxChannels + 1) * 4 elements large.
	    \param p_maxChannels Maximum number of channels supported.*/
	PPMOut(uint8_t         p_channels,
	       const uint16_t* p_input,
	       uint8_t*        p_work,
	       uint8_t         p_maxChannels);
	
	/*! \brief Sets up timers and interrupts.
	    \param p_a Use timer output pin A on true, output pin B on false.
	    \param p_invert Invert the signal on true.*/
	void start(bool p_a, bool p_invert = false);
	
	/*! \brief Sets channel count
	    \param p_channels Channel count.*/
	void setChannelCount(uint8_t p_channels);
	
	/*! \brief Gets channel count.
	    \return The amount of active channels.*/
	uint8_t getChannelCount() const;
	
	/*! \brief Sets pulse length
	    \param p_length Pulse length in microseconds.*/
	void setPulseLength(uint16_t p_length);
	
	/*! \brief Gets pulse length.
	    \return The current pulse length.*/
	uint16_t getPulseLength() const;
	
	/*! \brief Sets pause length
	    \param p_length Pause length in microseconds.*/
	void setPauseLength(uint16_t p_length);
	
	/*! \brief Gets pause length.
	    \return The current pause length.*/
	uint16_t getPauseLength() const;
	
	/*! \brief Updates channel timings, will be sent at next frame.*/
	void update();
	
	/*! \brief Handles timer interrupt.*/
	static void handleInterrupt();
	
private:
	/*! \brief Update the entire timings buffer. */
	void updateTimings();
	
	/*! \brief Internal interrupt handling. */
	void isr();
	
	uint16_t m_pulseLength; //!< Pulse length in timer ticks.
	uint16_t m_pauseLength; //!< End of frame length in timer ticks.
	
	uint8_t         m_channelCount;    //!< Number of active channels.
	const uint16_t* m_channels;        //!< External buffer with channel values, in microseconds.
	
	volatile uint16_t* m_channelTimings; //!< Timings per channel, in timer ticks.
	
	uint8_t   m_timingCount; //!< Number of active timings.
	uint8_t   m_timingPos;   //!< Current position in timings buffer.
	uint16_t* m_timings;     //!< Timing values in timer ticks.
	
	static PPMOut* s_instance; //!< Singleton instance
};
/** \example ppmout_example.pde
 * This is an example of how to use the PPMOut class.
 */

} // namespace end

#endif // INC_RC_PPMOUT_H
