#pragma once

#include <iostream>

#include "PacketQueueSimulator.hpp"

typedef double TrafficIntensity;
typedef double Factor;

/**
 * Generates statistics about a particular packet queue configuration
 * and stores them into a CSV file.
 */
class PacketQueueAnalyzer
{
private:

	PacketQueueSimulator *simulator;

public:

	/**
	 * Initializes the packet queue analyzer with the specified simulator from
	 * which to gather and report data. This class is only responsible for
	 * initiating simulations on a packet queue model for which size and
	 * transmission rate parameters have already been specified, whereby this
	 * class then generates packet arrival and monitoring events matching the
	 * parameters given to each invocation of one of the gatherDataFor() methods.
	 */
	PacketQueueAnalyzer(PacketQueueSimulator *simulator) : simulator(simulator)
	{
	}

	/**
	 * Adds the CSV headers to the start of the output.
	 */
	void writeHeaders(std::ostream *output);

	/**
	 * Runs a simulation of the packet queue for the specified packet generation
	 * parameters and a single value of rho
	 * (averagePacketLength * averagePacketsPerSecond / transmissionRate),
	 * adding a single CSV row of packet queue performance statistics to the output.
	 */
	void gatherDataFor(
		Seconds simulationTime,
		Bits averagePacketLength,
		Factor sampleRateFactor,
		TrafficIntensity rho,
		std::ostream *output
	);

	/**
	 * Runs a simulation of the packet queue for the specified packet generation
	 * parameters over a range of values for rho
	 * (averagePacketLength * averagePacketsPerSecond / transmissionRate),
	 * adding CSV rows of packet queue performance statistics to the output, each
	 * corresponding with a value of rho.
	 */
	void gatherDataFor(
		Seconds simulationTime,
		Bits averagePacketLength,
		Factor sampleRateFactor,
		TrafficIntensity rhoLower,
		TrafficIntensity rhoUpper,
		TrafficIntensity rhoStep,
		std::ostream *output
	);
};