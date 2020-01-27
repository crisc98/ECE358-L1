#pragma once

typedef int Bits;
typedef double Seconds;
typedef int BitsPerSecond;
typedef int Packets;

/**
 * An abstraction of a packet queue's state and statistics regarding its performance.
 */
class PacketQueueAbstraction
{
private:

	/**
	 * Resets all of the measured statistics so that a new set of measurements may
	 * be taken for a new set of packet queue events.
	 */
	void reset();

public:

	/**
	 * The packet queue's size, K, as measured in packets.
	 */
	Packets maxBufferSize;

	/**
	 * The current number of packets stored in the queue.
	 */
	Packets currentBufferSize;

	/**
	 * The cumulative number of packets in the queue across all observations.
	 */
	Packets cumulativeBufferSize;

	/**
	 * The rate in bits per second at which a packet can be serviced and transmitted.
	 * Used to determine the delay between a packets arrival, the previous packet's
	 * departure, and when the current packet will have finished being serviced.
	 */
	BitsPerSecond transmissionRate;

	Packets numArrivals;
	Packets numDepartures;
	Packets numDropped;
	int numIdles;
	int numObservations;

	/**
	 * True if the packet queue is currently empty and is not
	 * imminently serving a packet.
	 */
	bool isIdle;

	/**
	 * True if the packet queue is full and can accept no more packets.
	 */
	bool isFull;

	/**
	 * The time at which the last packet completed being transmitted.
	 */
	Seconds lastDeparture;

	PacketQueueAbstraction(
		Packets maxBufferSize,
		BitsPerSecond transmissionRate
	);

	/**
	 * Calculate E[N], the average number of packets in the queue across all observations.
	 */
	double getAverageBufferSize();

	/**
	 * Calculate P_idle, the fraction of all observations over which the queue
	 * was observed to be empty.
	 */
	double getQueueIdleRatio();

	/**
	 * Calculates P_loss, the ratio of the number of dropped packets to the total
	 * number of packets generated.
	 */
	double getPacketLossRatio();
};