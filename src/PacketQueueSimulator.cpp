#include "PacketQueueSimulator.hpp"

/**
 * Returns true if the specified time is between 0 seconds and the current simulation duration.
 */
bool PacketQueueSimulator::isWithinSimulationDuration(Seconds time)
{
	bool result = (time >= 0) && (time <= simulationDuration);
	return result;
}

/**
 * Adds an event to be processed at a particular time with respect to other events.
 *
 * Returns true if the specified event's time is within the simulation time;
 * if this is the case, This class takes _sole responsibility_ over the destruction of
 * the pointer passed in.
 *
 * If false is returned, it is up to the event's creator to delete it, whereby this
 * class _does not_ take responsibility for the destruction of the pointer passed in.
 */
bool PacketQueueSimulator::addEvent(PacketQueueEvent *event)
{
	if (isWithinSimulationDuration(event->time))
	{
		eventQueue.push(event);
		return true;
	}
	
	return false;
}

/**
 * Processes all of the events that were registered for this simulation;
 * some events may register further events to be processed in the future.
 */
void PacketQueueSimulator::runSimulation()
{
	while (!eventQueue.empty())
	{
		PacketQueueEvent *event = eventQueue.top();
		eventQueue.pop();
		event->process(this);
		delete event;
	}
}

/**
 * Removes and deletes all registered events without processing them.
 */
void PacketQueueSimulator::flush()
{
	while (!eventQueue.empty())
	{
		PacketQueueEvent *event = eventQueue.top();
		eventQueue.pop();
		delete event;
	}
}