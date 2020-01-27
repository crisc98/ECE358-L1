#pragma once

#include "PacketQueueSimulator.hpp"

class PacketQueueObservationEvent : public PacketQueueEvent
{
public:

	void process(PacketQueueSimulator *simulator);
};