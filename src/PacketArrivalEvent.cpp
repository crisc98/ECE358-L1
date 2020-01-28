#include "PacketArrivalEvent.hpp"
#include "PacketDepartureEvent.hpp"

/**
 * Attempts to generate a corresponding packet departure event for
 * this packet arrival event.
 */
void PacketArrivalEvent::generateDepartureEvent(PacketQueueSimulator *simulator)
{
	/**
	 * Only attempt to add a corresponding departure event if the last departure time was within
	 * the simulation's duration.
	 */
	if (simulator->isWithinSimulationDuration(simulator->packetQueue->lastDeparture))
	{
		Seconds departureTime;
		Seconds transmissionTime = ((double)length) / simulator->packetQueue->transmissionRate;

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
		 * If the packets arrive in one order, they will also depart in that order.
		 * If this packet's departure time is outside of the simulation time, the departure
		 * event will not be added and all subsequent packet arrival events will not
		 * attempt to add a departure event.
		 */
		simulator->packetQueue->lastDeparture = departureTime;

		/**
		 * Only add the departure event if it will occur within the simulation's duration.
		 */
		if (simulator->isWithinSimulationDuration(departureTime))
		{
			PacketDepartureEvent *departure = new PacketDepartureEvent(departureTime);
			simulator->addEvent(departure);
		}
	}
}

/**
 * Manages the logic for attempting to add a packet to the queue.
 */
void PacketArrivalEvent::process(PacketQueueSimulator *simulator)
{
	++simulator->packetQueue->numArrivals;

	/**
	 * Drop the packet if the queue is full, else, add it and try to register a
	 * corresponding departure event for it.
	 */
	if (simulator->packetQueue->isFull)
	{
		++simulator->packetQueue->numDropped;
	}
	else
	{
		++simulator->packetQueue->currentBufferSize;

		/**
		 * If maxBufferSize is negative, the packet queue is assumed to be infinite in size.
		 */
		if (
			(simulator->packetQueue->maxBufferSize >= 0) &&
			(simulator->packetQueue->currentBufferSize >= simulator->packetQueue->maxBufferSize)
			)
		{
			simulator->packetQueue->isFull = true;
		}

		generateDepartureEvent(simulator);
	}
}