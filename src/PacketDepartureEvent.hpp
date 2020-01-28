#pragma once

#include "PacketQueueSimulator.hpp"

/**
 * A discrete event representing the point in time at which a packet finishes being
 * serviced and transmitted and is removed from the packet queue.
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