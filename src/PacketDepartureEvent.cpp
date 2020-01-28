#include "PacketDepartureEvent.hpp"

/**
 * Manages the logic for transmitting a packet and removing it from the queue.
 */
void PacketDepartureEvent::process(PacketQueueSimulator *simulator)
{
	if (simulator->packetQueue->isIdle) return;

	++simulator->packetQueue->numDepartures;

	--simulator->packetQueue->currentBufferSize;

	/**
	 * The packet queue is idle once there are no more packets left in the
	 * buffer to be serviced; otherwise, it would be assumed that after this
	 * packet has finished being transmitted, the next packet will immediately
	 * start being serviced, though its departure event has yet to be triggered.
	 */
	if (simulator->packetQueue->currentBufferSize == 0)
	{
		simulator->packetQueue->isIdle = true;
	}

	simulator->packetQueue->isFull = false;
}