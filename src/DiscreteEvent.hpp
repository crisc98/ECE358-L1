#pragma once

/**
 *
 */
template <class TTime, class TState>
class DiscreteEvent
{
	typedef DiscreteEvent<TTime, TState> TypedDiscreteEvent;

public:

	/**
	 * The time at which this discrete event should be processed.
	 */
	TTime time;

	/**
	 * Creates a discrete event to be processed at the specified time.
	 */
	TypedDiscreteEvent(TTime time) : time(time)
	{
	}

	/**
	 * The processes the discrete event, allowing it to act upon the specific state.
	 */
	virtual void process(TState *state) = 0;

	/**
	 * Compares pointers to discrete events based on their time of occurrence.
	 * Namely, in a priority_queue, this comparator will always place the
	 * _earliest_ event at the top of the heap.
	 */
	class Comparator
	{
	public:

		bool operator()(TypedDiscreteEvent* lhs, TypedDiscreteEvent* rhs)
		{
			bool result = lhs->time > rhs->time;
			return result;
		}
	};
};