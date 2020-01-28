#include "PacketQueueAnalyzer.hpp"

/**
 * Adds the CSV headers to the start of the output.
 */
void PacketQueueAnalyzer::writeHeaders(std::ostream *output)
{
	*output << "rho,E[N],P_IDLE,P_LOSS\n" << std::flush;
}

/**
 * Runs a simulation of the packet queue for the specified packet generation
 * parameters and a single value of rho
 * (averagePacketLength * averagePacketsPerSecond / transmissionRate).
 * 
 * For the specified rho, the specified settings are used to generate all of
 * the packet arrival and packet queue observation events to be added to the
 * simulator, after which the simulator is run, at which point packet departure
 * events may be dynamically generated. After the simulation has finished
 * running, the generated packet queue performance statistics are written to
 * a CSV row.
 */
void PacketQueueAnalyzer::gatherDataFor(
	Seconds simulationTime,
	Bits averagePacketLength,
	Factor sampleRateFactor,
	TrafficIntensity rho,
	std::ostream *output
)
{
	// generate packet arrival events

	// generate packet queue observation events

	simulator->runSimulation();

	*output
		<< rho << ","
		<< simulator->packetQueue->getAverageBufferSize() << ","
		<< simulator->packetQueue->getQueueIdleRatio() << ","
		<< simulator->packetQueue->getPacketLossRatio() << "\n"
		<< std::flush;
}

/**
 * Runs a simulation of the packet queue for the specified packet generation
 * parameters over a range of values for rho
 * (averagePacketLength * averagePacketsPerSecond / transmissionRate).
 * See the other overload for more details.
 */
void PacketQueueAnalyzer::gatherDataFor(
	Seconds simulationTime,
	Bits averagePacketLength,
	Factor sampleRateFactor,
	TrafficIntensity rhoLower,
	TrafficIntensity rhoUpper,
	TrafficIntensity rhoStep,
	std::ostream *output
)
{
	TrafficIntensity rhoCurrent = rhoLower;

	while (rhoCurrent < rhoUpper)
	{
		gatherDataFor(
			simulationTime,
			averagePacketLength,
			sampleRateFactor,
			rhoCurrent,
			output
		);

		rhoCurrent += rhoStep;
	}
}