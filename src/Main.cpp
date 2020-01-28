#include <cstdlib>
#include <fstream>
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
		<< "f <sample rate factor (to be multiplied with lambda (average packets per second); double)>\n"
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
		<< "All inputs except for max buffer size must be positive, and all inputs must be nonzero.\n"
		<< "All settings start at a default, invalid value of 0.\n"
		<< "If rl > ru, they will automatically be flipped once the simulation is started.\n\n"
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

	if (*error_check || (requirePositive && (longTemp <= 0)))
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

	if (*error_check || (requirePositive && (doubleTemp <= 0)))
	{
		std::cout << errorMessage << std::flush;
		return false;
	}

	*doubleVariable = doubleTemp;

	return true;
}

/**
 * Double-checks that all the data is valid.
 */
bool validateSettings(
	Seconds simulationTime,
	Bits averagePacketLength,
	Factor sampleRateFactor,
	BitsPerSecond transmissionRate,
	Packets maxBufferSize,
	TrafficIntensity rhoSingle,
	TrafficIntensity rhoLower,
	TrafficIntensity rhoUpper,
	TrafficIntensity rhoStep
)
{
	bool dataMissing = false;
	if (simulationTime <= 0)
	{
		std::cout << "t <simulation time in seconds (double)> has not yet been successfully entered.\n\n";
		dataMissing = true;
	}
	if (averagePacketLength <= 0)
	{
		std::cout << "l <average packet length in bits (long)> has not yet been successfully entered.\n\n";
		dataMissing = true;
	}
	if (sampleRateFactor <= 0)
	{
		std::cout << "f <sample rate factor (to be multiplied with lambda (average packets per second); double)> has not yet been successfully entered.\n\n";
		dataMissing = true;
	}
	if (transmissionRate <= 0)
	{
		std::cout << "c <transmission rate in bits per second (long)> has not yet been successfully entered.\n\n";
		dataMissing = true;
	}
	if (maxBufferSize == 0)
	{
		std::cout << "k <max buffer size in packets (negative for infinity; long)> has not yet been successfully entered.\n\n";
		dataMissing = true;
	}
	if (rhoSingle <= 0)
	{
		// rhoSingle is mutually exclusive with rhoLower, rhoUpper and rhoStep
		bool useRangeOfRho = true;
		if (rhoLower <= 0)
		{
			std::cout << "rl <rho lower (l * lambda / c; double)> has not yet been successfully entered.\n\n";
			useRangeOfRho = false;
		}
		if (rhoUpper <= 0)
		{
			std::cout << "ru <rho upper (l * lambda / c; double)> has not yet been successfully entered.\n\n";
			useRangeOfRho = false;
		}
		if (rhoStep <= 0)
		{
			std::cout << "rs <rho step size (l * lambda / c; double)> has not yet been successfully entered.\n\n";
			useRangeOfRho = false;
		}
		if (!useRangeOfRho)
		{
			std::cout << "Alternatively, ri <use a single value of rho (l * lambda / c; double)> has not yet been successfully entered.\n\n";
			dataMissing = true;
		}
	}
	std::cout << std::flush;
	return !dataMissing;
}

/**
 * Attempts to run an analysis of packet queue performance using the specified settings and analyzer.
 * The results may be outputted either to the console or to a file.
 */
void runAnalysis(
	PacketQueueAnalyzer *analyzer,
	Seconds simulationTime,
	Bits averagePacketLength,
	Factor sampleRateFactor,
	BitsPerSecond transmissionRate,
	Packets maxBufferSize,
	TrafficIntensity rhoSingle,
	TrafficIntensity rhoLower,
	TrafficIntensity rhoUpper,
	TrafficIntensity rhoStep,
	bool outputToFile
)
{
	if (validateSettings(
		simulationTime,
		averagePacketLength,
		sampleRateFactor,
		transmissionRate,
		maxBufferSize,
		rhoSingle,
		rhoLower,
		rhoUpper,
		rhoStep
	))
	{
		std::ostream *output = nullptr;
		std::ofstream *fileOutput = nullptr;

		// select between a console or file output stream
		if (outputToFile)
		{
			std::string filename;
			std::cin >> filename;
			filename.append(".csv");
			fileOutput = new std::ofstream(filename);
			output = fileOutput;
			std::cout << "Running the simulation using the current settings and outputting to " << filename << ":\n\n" << std::flush;
		}
		else
		{
			output = &std::cout;
			std::cout << "Running the simulation using the current settings and outputting to console:\n\n" << std::flush;
		}

		// write the current settings to the output as a CSV row with header
		*output
			<< "t,l,f,c,k,rs,rl,ru,rs\n"
			<< simulationTime << ","
			<< averagePacketLength << ","
			<< sampleRateFactor << ","
			<< transmissionRate << ","
			<< maxBufferSize << ","
			<< rhoSingle << ","
			<< rhoLower << ","
			<< rhoUpper << ","
			<< rhoStep << "\n"
			<< std::flush;

		// add statistical data for each rho value as CSV rows with a header
		analyzer->writeHeaders(output);
		if (rhoSingle > 0)
		{
			analyzer->gatherDataFor(
				simulationTime,
				averagePacketLength,
				sampleRateFactor,
				rhoSingle,
				output
			);
		}
		else
		{
			analyzer->gatherDataFor(
				simulationTime,
				averagePacketLength,
				sampleRateFactor,
				rhoLower,
				rhoUpper,
				rhoStep,
				output
			);
		}

		// clean up the file output stream if needed
		if (outputToFile && (fileOutput != nullptr))
		{
			fileOutput->close();
			delete fileOutput;
		}
	}
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
	Seconds simulationTime = 0;
	Bits averagePacketLength = 0;
	Factor sampleRateFactor = 0;
	BitsPerSecond transmissionRate = 0;
	Packets maxBufferSize = 0;
	TrafficIntensity rhoSingle = 0;
	TrafficIntensity rhoLower = 0;
	TrafficIntensity rhoUpper = 0;
	TrafficIntensity rhoStep = 0;

	bool succeeded = false;

	// I/O loop implementing what is described by showUsage()
	while (true)
	{
		std::string str;
		char input;
		std::cin >> input;
		switch (input)
		{
		case 'e': // output exponential distribution performance to console
			break;
		case 't': // set the simulation time
			getDouble(&simulationTime, true, "Invalid simulation time; please enter a valid positive nonzero decimal value (double) in seconds.\n\n");
			break;
		case 'l': // set the average packet length
			getLong(&averagePacketLength, true, "Invalid average packet length; please enter a valid positive nonzero integer (long) in bits.\n\n");
			break;
		case 'f': // set the sample rate factor
			getDouble(&sampleRateFactor, true, "Invalid sample rate factor; please enter a valid positive nonzero decimal value (double) to be multiplied with lambda.\n\n");
			break;
		case 'c': // set the packet queue server's transmission rate
			getLong(&transmissionRate, true, "Invalid transmission rate; please enter a valid positive nonzero integer (long) in bits per second.\n\n");
			break;
		case 'k': // set the max buffer size
			getLong(&maxBufferSize, false, "Invalid max buffer size; please enter a valid nonzero integer (long) in packets.\n\n");
			break;
		case 'r': // set rho
			std::cin >> input;
			switch (input)
			{
			case 'i': // use a single value of rho
				succeeded = getDouble(&rhoSingle, true, "Invalid single value for rho; please enter a valid positive nonzero decimal value (double) as l * lambda / c.\n\n");
				if(succeeded)
				{
					// this option is mutually exclusive of the others
					rhoLower = 0;
					rhoUpper = 0;
					rhoStep = 0;
				}
				break;
			case 'l': // set the lower bound for rho
				succeeded = getDouble(&rhoLower, true, "Invalid lower value for rho; please enter a valid positive nonzero decimal value (double) as l * lambda / c.\n\n");
				if (succeeded)
				{
					// this option is mutually exclusive of the first
					rhoSingle = 0;
				}
				break;
			case 'u': // set the upper bound for rho
				succeeded = getDouble(&rhoUpper, true, "Invalid upper value for rho; please enter a valid positive nonzero decimal value (double) as l * lambda / c.\n\n");
				if (succeeded)
				{
					// this option is mutually exclusive of the first
					rhoSingle = 0;
				}
				break;
			case 's': // set the step size for rho
				succeeded = getDouble(&rhoStep, true, "Invalid value for rho step size; please enter a valid positive nonzero decimal value (double) as l * lambda / c.\n\n");
				if (succeeded)
				{
					// this option is mutually exclusive of the first
					rhoSingle = 0;
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
				runAnalysis(
					&analyzer,
					simulationTime,
					averagePacketLength,
					sampleRateFactor,
					transmissionRate,
					maxBufferSize,
					rhoSingle,
					rhoLower,
					rhoUpper,
					rhoStep,
					false
				);
				break;
			case 'f': // output the CSV results to a file
				runAnalysis(
					&analyzer,
					simulationTime,
					averagePacketLength,
					sampleRateFactor,
					transmissionRate,
					maxBufferSize,
					rhoSingle,
					rhoLower,
					rhoUpper,
					rhoStep,
					true
				);
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