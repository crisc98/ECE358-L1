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

	/**
	 * The number of seconds for which the simulator will run.
	 * This controls whether an event being registered to be processed at a particular
	 * time will be accepted.
	 * This may be used to decide if an event should be created at all.
	 */
	Seconds simulationDuration;

	PacketQueueAbstraction *packetQueue;

	/**
	 * Creates a packet queue simulator using the specified packet queue abstraction.
	 * The specified abstraction will define how the simulator packet queue's state
	 * is managed and can be modified externally to change the simulation parameters.
	 * All events registered via addEvent() are assumed to "act upon" the state of
	 * this abstraction in some way.
	 */
	PacketQueueSimulator(PacketQueueAbstraction *packetQueue) : simulationDuration(0), packetQueue(packetQueue)
	{
	}

	/**
	 * Returns true if the specified time is between 0 seconds and the current simulation duration.
	 */
	bool isWithinSimulationDuration(Seconds time);

	/**
	 * Adds an event to be processed at a particular time with respect to other events,
	 * _only_ if it is within the simulation's duration; this ensures that no events
	 * occurring _outside_ of the simulation's duration will contribute to the packet queue's
	 * measured performance statistics.
	 * 
	 * Returns true if the specified event's time is within the simulation time;
	 * if this is the case, This class takes _sole responsibility_ over the destruction of
	 * the pointer passed in.
	 * 
	 * If false is returned, it is up to the event's creator to delete it, whereby this
	 * class _does not_ take responsibility for the destruction of the pointer passed in.
	 * 
	 * See isWithinSimulationDuration().
	 */
	bool addEvent(PacketQueueEvent *event);

	/**
	 * Runs the simulation until all registered events have been processed and deleted.
	 */
	void runSimulation();

	/**
	 * Removes and deletes all registered events without processing them.
	 * This does not need to be called if runSimulation() has already been called.
	 */
	void flush();

	/**
	 * Ensures that all registered event objects have been destroyed once this class is destroyed.
	 */
	~PacketQueueSimulator()
	{
		flush();
	}
};