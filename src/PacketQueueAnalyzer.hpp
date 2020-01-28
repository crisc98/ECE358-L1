#pragma once

#include "PacketQueueSimulator.hpp"

typedef double TrafficeIntensity;
typedef long Factor;

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
	 * Runs a simulation of the packet queue for the specified packet generation
	 * parameters and a single value of rho
	 * (averagePacketLength * averagePacketsPerSecond / transmissionRate).
	 */
	void gatherDataFor(
		Seconds simulationTime,
		Bits averagePacketLength,
		Factor sampleRateFactor,
		TrafficeIntensity rho
	);

	/**
	 * Runs a simulation of the packet queue for the specified packet generation
	 * parameters over a range of values for rho
	 * (averagePacketLength * averagePacketsPerSecond / transmissionRate).
	 */
	void gatherDataFor(
		Seconds simulationTime,
		Bits averagePacketLength,
		Factor sampleRateFactor,
		TrafficeIntensity rhoLower,
		TrafficeIntensity rhoUpper,
		TrafficeIntensity rhoStep
	);
};