#pragma once

#include "PacketQueueSimulator.hpp"

/**
 *
 */
class PacketArrivalEvent : public PacketQueueEvent
{
public:

	/**
	 * The length of the packet in bits.
	 */
	Bits length;

	PacketArrivalEvent(Seconds time, Bits length);

	/**
	 * Manages the logic for attempting to add a packet to the queue.
	 */
	void process(PacketQueueSimulator *simulator);
};