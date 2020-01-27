# CMakeList.txt : CMake project for ECE358-L1, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (
	ECE358-L1
	"DiscreteEvent.hpp"
	"Main.cpp"
	"PacketArrivalEvent.cpp"
	"PacketArrivalEvent.hpp"
	"PacketDepartureEvent.cpp"
	"PacketDepartureEvent.hpp"
	"PacketQueueAbstraction.cpp"
	"PacketQueueAbstraction.hpp"
	"PacketQueueAnalyzer.cpp"
	"PacketQueueAnalyzer.hpp"
	"PacketQueueObservationEvent.cpp"
	"PacketQueueObservationEvent.hpp"
	"PacketQueueSimulator.cpp"
	"PacketQueueSimulator.hpp"
	)

# TODO: Add tests and install targets if needed.
