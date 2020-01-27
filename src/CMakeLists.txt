# CMakeList.txt : CMake project for ECE358-L1, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (
	ECE358-L1
	"ExponentialDistribution.h"
	"Main.cpp"
	"Packet.cpp"
	"Simulator.cpp"
	"Simulator.h"
	)

# TODO: Add tests and install targets if needed.
