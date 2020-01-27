#pragma once

#include <functional>
#include <queue>
#include <vector>

#include "PacketQueueAbstraction.hpp"
#include "DiscreteEvent.hpp"

class PacketQueueSimulator;

typedef DiscreteEvent<Seconds, PacketQueueSimulator> PacketQueueEvent;

/**
 * Processes a set of discrete packet queue events that act on the
 * packet queue's state.
 *
 * The simulator logic is only responsible for making the events happen in order,
 * whereby it is otherwise independent of any methods of gathering statistics
 * about its performance. It likewise does not know exactly what _kinds_ of
 * "packet queue events" it will be running, whereby it delegates these details
 * to the subclasses themselves per the "strategy pattern".
 */
class PacketQueueSimulator
{
private:

	/**
	 * Organizes a collection of discrete events in the order of their expected
	 * occurrence. The top of the priority queue/heap will point to the event
	 * with the _earliest_ scheduled time, whereby successive pops of the queue
	 * will cause events to be processed in chronological order while allowing
	 * some events to add new events to be processed sometime in the future.
	 */
	std::priority_queue<
		PacketQueueEvent*,
		std::vector<PacketQueueEvent*>,
		PacketQueueEvent::Comparator> eventQueue;

public:

	PacketQueueAbstraction packetQueue;

	PacketQueueSimulator(PacketQueueAbstraction packetQueue) : packetQueue(packetQueue)
	{
	}

	/**
	 * Adds an event to be processed at a particular time with respect to other events.
	 */
	void addEvent(PacketQueueEvent *event);

	/**
	 * Runs the simulation until all registered events have been processed.
	 */
	void runSimulation();

	/**
	 * Removes and deletes all registered events without processing them.
	 */
	void flush();
};