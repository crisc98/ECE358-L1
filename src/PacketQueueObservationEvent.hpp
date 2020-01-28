#pragma once

#include "PacketQueueSimulator.hpp"

/**
 * A discrete event respresenting a point in time at which the packet queue's
 * state is sampled.
 */
class PacketQueueObservationEvent : public PacketQueueEvent
{
public:

	/**
	 * Logs information about the packet queue's current state.
	 */
	void process(PacketQueueSimulator *simulator);
};