#include <cstdlib>
#include <iostream>
#include <string>

#include "PacketQueueAnalyzer.hpp"

/**
 * Displays information on how to use this program and access each feature.
 */
void showUsage()
{
	std::cout 
		<< "ECE358-L1 Commands:\n\n"
		<< "e - output exponential distribution performance to console\n"
		<< "t <simulation time in seconds (double)>\n"
		<< "l <average packet length in bits (long)>\n"
		<< "f <sample rate factor (multiple of lambda (average packets per second); long)>\n"
		<< "c <transmission rate in bits per second (long)>\n"
		<< "k <max buffer size in packets (negative for infinity; long)>\n"
		<< "ri <use a single value of rho (l * lambda / c; double)>\n"
		<< "rl <rho lower (l * lambda / c; double)>\n"
		<< "ru <rho upper (l * lambda / c; double)>\n"
		<< "rs <rho step size (l * lambda / c; double)>\n"
		<< "s - show the current settings\n"
		<< "oc - run the simulator and output CSV data to console\n"
		<< "of <filename without extension> - run the simulator and output CSV data to a file\n"
		<< "q - end the program>\n"
		<< "h - show this menu\n\n"
		<< "Inputs are error-checked and will revert the setting to its previous value if there is a problem.\n"
		<< "Except for the max buffer size, -1 is the default invalid value.\n\n"
		<< std::flush;
}

/**
 * Read a long integer from the console with error checking and message.
 * The specified longVariable is not updated unless the input string is successfully parsed.
 * Returns true if the read succeeded.
 * See https://stackoverflow.com/questions/24785362/converting-string-to-integer-double-float-without-having-to-catch-exceptions.
 */
bool getLong(long *longVariable, bool requirePositive, const char *errorMessage)
{
	std::string str;
	std::cin >> str;
	char *error_check;
	long longTemp = std::strtol(str.c_str(), &error_check, 10);

	if (*error_check || (requirePositive && (longTemp < 0)))
	{
		std::cout << errorMessage << std::flush;
		return false;
	}

	*longVariable = longTemp;

	return true;
}
/**
 * Read a double-precision floating point number from the console with error checking and message.
 * The specified doubleVariable is not updated unless the input string is successfully parsed.
 * Returns true if the read succeeded.
 * See https://stackoverflow.com/questions/24785362/converting-string-to-integer-double-float-without-having-to-catch-exceptions.
 */
bool getDouble(double *doubleVariable, bool requirePositive, const char *errorMessage)
{
	std::string str;
	std::cin >> str;
	char *error_check;
	double doubleTemp = std::strtod(str.c_str(), &error_check);

	if (*error_check || (requirePositive && (doubleTemp < 0)))
	{
		std::cout << errorMessage << std::flush;
		return false;
	}

	*doubleVariable = doubleTemp;

	return true;
}

/**
 * Encapsulates the main program loop for gathering data about different packet queue
 * configurations' performance.
 */
int main(int argc, char *argv[])
{
	showUsage();

	// the packet queue state
	PacketQueueAbstraction packetQueue;

	// runs events against the packet queue state
	PacketQueueSimulator simulator(&packetQueue); 

	// runs the experiments using the simulator and logs the data
	PacketQueueAnalyzer analyzer(&simulator);

	// simulation settings; these may be updated throughout the session
	Seconds simulationTime = -1;
	Bits averagePacketLength = -1;
	Factor sampleRateFactor = -1;
	BitsPerSecond transmissionRate = -1;
	Packets maxBufferSize = -1;
	TrafficeIntensity rhoSingle = -1;
	TrafficeIntensity rhoLower = -1;
	TrafficeIntensity rhoUpper = -1;
	TrafficeIntensity rhoStep = -1;

	bool succeeded = false;

	// I/O loop implementing what is described by showUsage()
	while (true)
	{
		char input;
		std::cin >> input;
		switch (input)
		{
		case 'e': // output exponential distribution performance to console
			break;
		case 't': // set the simulation time
			getDouble(&simulationTime, true, "Invalid simulation time; please enter a valid positive decimal value (double) in seconds.\n\n");
			break;
		case 'l': // set the average packet length
			getLong(&averagePacketLength, true, "Invalid average packet length; please enter a valid positive integer (long) in bits.\n\n");
			break;
		case 'f': // set the sample rate factor
			getLong(&sampleRateFactor, true, "Invalid sample rate factor; please enter a valid positive integer (long) as a factor of lambda.\n\n");
			break;
		case 'c': // set the packet queue server's transmission rate
			getLong(&transmissionRate, true, "Invalid transmission rate; please enter a valid positive integer (long) in bits per second.\n\n");
			break;
		case 'k': // set the max buffer size
			getLong(&maxBufferSize, false, "Invalid max buffer size; please enter a valid positive integer (long) in packets.\n\n");
			break;
		case 'r': // set rho
			std::cin >> input;
			switch (input)
			{
			case 'i': // use a single value of rho
				succeeded = getDouble(&rhoSingle, true, "Invalid single value for rho; please enter a valid positive decimal value (double) as l * lambda / c.\n\n");
				if(succeeded)
				{
					// this option is mutually exclusive of the others
					rhoLower = -1;
					rhoUpper = -1;
					rhoStep = -1;
				}
				break;
			case 'l': // set the lower bound for rho
				succeeded = getDouble(&rhoLower, true, "Invalid lower value for rho; please enter a valid positive decimal value (double) as l * lambda / c.\n\n");
				if (succeeded)
				{
					// this option is mutually exclusive of the first
					rhoSingle = -1;
				}
				break;
			case 'u': // set the upper bound for rho
				succeeded = getDouble(&rhoUpper, true, "Invalid upper value for rho; please enter a valid positive decimal value (double) as l * lambda / c.\n\n");
				if (succeeded)
				{
					// this option is mutually exclusive of the first
					rhoSingle = -1;
				}
				break;
			case 's': // set the step size for rho
				succeeded = getDouble(&rhoStep, true, "Invalid value for rho step size; please enter a valid positive decimal value (double) as l * lambda / c.\n\n");
				if (succeeded)
				{
					// this option is mutually exclusive of the first
					rhoSingle = -1;
				}
				break;
			default:
				std::cout << "Invalid command; enter \'h\' to show the list of commands.\n\n" << std::flush;
			}
			break;
		case 's': // show the settings
			std::cout
				<< "Settings:\n\n"
				<< "t <simulation time in seconds (double)>: " << simulationTime << "\n"
				<< "l <average packet length in bits (long)>: " << averagePacketLength << "\n"
				<< "f <sample rate factor (multiple of lambda (average packets per second); long)>: " << sampleRateFactor << "\n"
				<< "c <transmission rate in bits per second (long)>: " << transmissionRate << "\n"
				<< "k <max buffer size in packets (negative for infinity; long)>: " << maxBufferSize << "\n"
				<< "ri <use a single value of rho (l * lambda / c; double)>: " << rhoSingle << "\n"
				<< "rl <rho lower (l * lambda / c; double)>: " << rhoLower << "\n"
				<< "ru <rho upper (l * lambda / c; double)>: " << rhoUpper << "\n"
				<< "rs <rho step size (l * lambda / c; double)>: " << rhoStep << "\n\n"
				<< std::flush;
			break;
		case 'o': // run the simulation and output the results
			std::cin >> input;
			switch (input)
			{
			case 'c': // output the CSV results to the console
				std::cout << "Running the simulation using the current settings and outputting to console:\n\n" << std::flush;
				break;
			case 'f': // output the CSV results to a file
				std::cout << "Running the simulation using the current settings and outputting to file:\n\n" << std::flush;
				break;
			default:
				std::cout << "Invalid command; enter \'h\' to show the list of commands.\n\n" << std::flush;
			}
			break;
		case 'q': // end the program; let the destructors do their thing
			return 0;
		case 'h': // show the list of commands
			showUsage();
			break;
		default:
			std::cout << "Invalid command; enter \'h\' to show the list of commands.\n\n" << std::flush;
		}
	}
}