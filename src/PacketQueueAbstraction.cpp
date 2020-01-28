#include "PacketQueueAbstraction.hpp"

/**
 * Calculate E[N], the average number of packets in the queue across all observations.
 */
double PacketQueueAbstraction::getAverageBufferSize()
{
	double averageBufferSize = ((double)cumulativeBufferSize) / ((double)numObservations);
	return averageBufferSize;
}

/**
 * Calculate P_idle, the fraction of all observations over which the queue
 * was observed to be empty.
 */
double PacketQueueAbstraction::getQueueIdleRatio() {
	double queueIdleRatio = ((double)numIdles) / ((double)numObservations);
	return queueIdleRatio;
}

/**
 * Calculates P_loss, the ratio of the number of dropped packets to the total
 * number of packets generated.
 */
double PacketQueueAbstraction::getPacketLossRatio() {
	double packetLossRatio = ((double)numDropped) / ((double)numArrivals);
	return packetLossRatio;
}

/**
 * Resets all of the measured statistics so that a new set of measurements may
 * be taken for a new set of packet queue events, leaving maxBufferSize and
 * transmissionRate unchanged; those may be edited manually.
 *
 * The maxBufferSize and transmissionRate MUST be set and this method called
 * before running a new simulation of the packet queue.
 */
void PacketQueueAbstraction::reset()
{
	cumulativeBufferSize = 0;
	currentBufferSize = 0;
	numArrivals = 0;
	numDepartures = 0;
	numDropped = 0;
	numIdles = 0;
	numObservations = 0;
	isIdle = true;
	isFull = false;
	lastDeparture = 0;
}