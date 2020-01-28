#include "PacketArrivalEvent.hpp"
#include "PacketDepartureEvent.hpp"

PacketArrivalEvent::PacketArrivalEvent(
	Seconds time,
	Bits length) :
	PacketQueueEvent(time),
	length(length)
{
}

/**
 * Manages the logic for attempting to add a packet to the queue.
 */
void PacketArrivalEvent::process(PacketQueueSimulator *simulator)
{
	++simulator->packetQueue->numArrivals;

	if (simulator->packetQueue->isFull)
	{
		++simulator->packetQueue->numDropped;
	}
	else
	{
		Seconds departureTime;
		Seconds transmissionTime = ((double) length) / simulator->packetQueue->transmissionRate;

		if (simulator->packetQueue->isIdle)
		{
			/**
			 * When idle, the packet is immediately serviced and will take some time
			 * to be transmitted, such proportional to the transmission rate and the
			 * packet's length in bits.
			 */
			departureTime = time + transmissionTime;
			simulator->packetQueue->isIdle = false;
		}
		else
		{
			/**
			 * If the server is busy, the current packet will have to wait until the
			 * previous packet has finished being transmitted.
			 */
			departureTime = simulator->packetQueue->lastDeparture + transmissionTime;
		}

		/**
		 * Store the calculated departure time that all subsequent packets may cumulatively
		 * add their departure time on top of.
		 */
		simulator->packetQueue->lastDeparture = departureTime;

		++simulator->packetQueue->currentBufferSize;

		/**
		 * If maxBufferSize is negative, the packet queue is assumed to be infinite in size.
		 */
		if (
			(simulator->packetQueue->maxBufferSize >= 0) &&
			(simulator->packetQueue->currentBufferSize < simulator->packetQueue->maxBufferSize)
			)
		{
			simulator->packetQueue->isFull = true;
		}

		PacketDepartureEvent *departure = new PacketDepartureEvent(departureTime);
		simulator->addEvent(departure);
	}
}