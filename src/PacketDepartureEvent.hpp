#pragma once

#include "PacketQueueSimulator.hpp"

/**
 *
 */
class PacketDepartureEvent : public PacketQueueEvent
{
public:

	PacketDepartureEvent(Seconds time) : PacketQueueEvent(time)
	{
	}

	/**
	 * Manages the logic for transmitting a packet and removing it from the queue.
	 */
	void process(PacketQueueSimulator *simulator);
};