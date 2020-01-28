#pragma once

#include "PacketQueueSimulator.hpp"

/**
 * A discrete event representing the point in time at which a packet has
 * been received at the packet switch and is to be added to the queue.
 */
class PacketArrivalEvent : public PacketQueueEvent
{
public:

	/**
	 * The length of the packet in bits.
	 */
	Bits length;

	PacketArrivalEvent(
		Seconds time,
		Bits length
	);

	/**
	 * Manages the logic for attempting to add a packet to the queue.
	 */
	void process(PacketQueueSimulator *simulator);
};