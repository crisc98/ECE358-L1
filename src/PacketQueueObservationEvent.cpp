#include "PacketQueueObservationEvent.hpp"

/**
 * Logs information about the packet queue's current state.
 */
void PacketQueueObservationEvent::process(PacketQueueSimulator *simulator)
{
	simulator->packetQueue->cumulativeBufferSize += simulator->packetQueue->currentBufferSize;
	
	if (simulator->packetQueue->isIdle)
	{
		++simulator->packetQueue->numIdles;
	}

	++simulator->packetQueue->numObservations;
}