#include <random>

#include "PacketArrivalEvent.hpp"
#include "PacketQueueAnalyzer.hpp"
#include "PacketQueueObservationEvent.hpp"

/**
 * Adds the CSV headers to the start of the output.
 */
void PacketQueueAnalyzer::writeHeaders(std::ostream *output)
{
	*output << "rho,numArrivals,numDepartures,numDropped,numIdles,numObservations,E[N],P_IDLE,P_LOSS\n" << std::flush;
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
	Bits averagePacketLength,
	Factor sampleRateFactor,
	TrafficIntensity rho,
	std::ostream *output
)
{
	typedef double AveragePacketsPerSecond;
	typedef double AveragePacketLength;
	typedef double AverageObservationsPerSecond;

	// initialize the simulator
	simulator->flush();
	simulator->packetQueue->reset();

	// initialize random number generation; see https://en.cppreference.com/w/cpp/numeric/random/exponential_distribution
	std::random_device rd;
	std::mt19937_64 gen(rd());

	// generate packet arrival events

	/**
	 * The average seconds per packet (inter-arrival time) is equal to 1 / lambda.
	 * rho is equal to L * lambda / C where L is the average packet length in bits
	 * and C is the transmission rate in bits per second.
	 */
	AveragePacketsPerSecond lambda = 
		rho
		* ((double)simulator->packetQueue->transmissionRate)
		/ ((double)averagePacketLength);

	// an upside-down 'L' if you get the gist
	double bigGamma = 1.0 / (AveragePacketLength)averagePacketLength;

	std::exponential_distribution<AveragePacketsPerSecond> interArrivalTime(lambda);
	std::exponential_distribution<AveragePacketLength> packetLength(bigGamma);
	
	/**
	 * Create packet arrival events for packets whose inter-arrival time and packet length
	 * follow an exponential distribution.
	 */
	Seconds lastArrivalTime = 0;
	while (true)
	{
		Seconds currentInterArrivalTime = interArrivalTime(gen);
		Seconds currentArrivalTime = lastArrivalTime + currentInterArrivalTime;
		lastArrivalTime = currentArrivalTime;
		if (simulator->isWithinSimulationDuration(currentArrivalTime))
		{
			Bits currentPacketLength = (Bits)packetLength(gen);
			PacketArrivalEvent *arrival = new PacketArrivalEvent(
				currentArrivalTime,
				currentPacketLength
			);
			simulator->addEvent(arrival);
		}
		else
		{
			break;
		}
	}

	// generate packet queue observation events

	/**
	 * The average seconds per observation (inter-observational time) is equal to 1 / alpha.
	 */
	AverageObservationsPerSecond alpha = lambda * sampleRateFactor;

	std::exponential_distribution<AveragePacketsPerSecond> interObservationTime(alpha);
	
	/**
	 * Create packet queue observation events for observations whose inter-observation time
	 * follows an exponential distribution.
	 */
	Seconds lastObservationTime = 0;
	while (true)
	{
		Seconds currentInterObservationTime = interObservationTime(gen);
		Seconds currentObservationTime = lastObservationTime + currentInterObservationTime;
		lastObservationTime = currentObservationTime;
		if (simulator->isWithinSimulationDuration(currentObservationTime))
		{
			PacketQueueObservationEvent *observation = new PacketQueueObservationEvent(currentObservationTime);
			simulator->addEvent(observation);
		}
		else
		{
			break;
		}
	}

	/**
	 * Process all generated events until all packet queue performance statistics
	 * for the simulation's duration have been generated.
	 */
	simulator->runSimulation();

	// write the results to a CSV row
	*output
		<< rho << ","
		<< simulator->packetQueue->numArrivals << ","
		<< simulator->packetQueue->numDepartures << ","
		<< simulator->packetQueue->numDropped << ","
		<< simulator->packetQueue->numIdles << ","
		<< simulator->packetQueue->numObservations << ","
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
 * 
 * Swaps rhoLower with rhoUpper if rhoLower is greater than rhoUpper.
 * Immediately aborts if rhoStep is 0.
 */
void PacketQueueAnalyzer::gatherDataFor(
	Bits averagePacketLength,
	Factor sampleRateFactor,
	TrafficIntensity rhoLower,
	TrafficIntensity rhoUpper,
	TrafficIntensity rhoStep,
	std::ostream *output
)
{
	if (rhoStep == 0)
	{
		return;
	}

	if (rhoLower > rhoUpper)
	{
		std::swap(rhoLower, rhoUpper);
	}

	TrafficIntensity rhoCurrent = rhoLower;

	while (rhoCurrent <= rhoUpper)
	{
		gatherDataFor(
			averagePacketLength,
			sampleRateFactor,
			rhoCurrent,
			output
		);

		rhoCurrent += rhoStep;
	}
}