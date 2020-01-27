#include "PacketQueueSimulator.hpp"

/**
 * Registers the specified event to be processed at a particular time.
 */
void PacketQueueSimulator::addEvent(PacketQueueEvent *event)
{
	eventQueue.push(event);
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