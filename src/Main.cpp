#include <cstdlib>

#include "PacketQueueSimulator.hpp"


void showUsage()
{

}

/**
 *
 */
int main(int argc, char *argv[])
{
	if (argc == 2)
	{
		// Run the exponential distribution test.
	}

	if (argc < 8)
	{
		showUsage();
		return -1;
	}

	Seconds simulationTime = std::atof(argv[1]);
	double averagePacketsPerSecond = std::atof(argv[2]);
	Bits averagePacketLength = std::atoi(argv[3]);
	int sampleRateFactor = std::atoi(argv[4]);
	BitsPerSecond transmissionRate = std::atoi(argv[5]);
	Packets maxBufferSize = std::atoi(argv[6]);
	double rhoLower = std::atof(argv[7]);
	double rhoUpper;
	double rhoStep;

	if (argc >= 10)
	{
		rhoUpper = std::atof(argv[8]);
		rhoStep = std::atof(argv[9]);
	}
	else if (argc == 8)
	{
		rhoUpper = rhoLower;
		rhoStep = 0;
	}
	else
	{
		showUsage();
		return -1;
	}

	PacketQueueAbstraction packetQueue(maxBufferSize, transmissionRate);
	PacketQueueSimulator simulator(packetQueue);



	simulator.runSimulation();

}