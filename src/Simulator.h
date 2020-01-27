#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "Packet.h"
#include "ExponentialDistribution.h"

using namespace std;
class Simulator 
{
    private:
    
    vector<int> queueSizes;
    vector<Packet> queue;
    enum Event {NONE, ARRIVAL, OBSERVER};
    vector<Event> eventQueue;
    long packetsPerSecond;
    long avgPacketLength;
    long transmissionRate;
    long totalTicks = 0;
    long nextTickArrival = 0;
    long nextTickObserver = 0;
    long nextTickDeparture = 0;
    long idleTime = 0;
    long numArrivals = 0;
    long numDepartures = 0;
    long numObservations = 0;

    public:
    Simulator(long pps, double L, double rate) 
    {
        packetsPerSecond = pps;
        avgPacketLength = L;
        transmissionRate = rate;
    }

    void generateEvents() {
        //Insert arrival event, wait randVar*totalTicks ticks and insert again
        for(int i = 0; i < totalTicks; i++){
            eventQueue.push_back(NONE);
        }
        for(int i = 0; i < totalTicks; i++){
            if(i == nextTickArrival) {
                eventQueue[i] = ARRIVAL;
                ExponentialDistribution e;
                double randomVariable = e.generateRandVar(packetsPerSecond);
                nextTickArrival = (long) i + (long) (randomVariable * totalTicks);              
            }
            if(i == nextTickObserver) {
                eventQueue[i+1] = OBSERVER;
                ExponentialDistribution e;
                double randomVariable = e.generateRandVar(5*packetsPerSecond);
                nextTickObserver = (long) i + (long) (randomVariable * totalTicks);
            }
        }
        cout << "Events Generated" << endl;
    }

    double generatePacketLength(){
        ExponentialDistribution e;
        double retVal = e.generateRandVar(avgPacketLength);
        return 1/retVal;
    }
    
    void startSimulation(long ticks) {
        totalTicks = ticks;
        Packet* currentPacket;
        generateEvents();
        for (long tick = 0; tick < ticks; tick++) {
            switch(eventQueue[tick]) {
                case ARRIVAL:
                    queue.push_back(Packet(tick, generatePacketLength()));
                    numArrivals++;
                    break;
                case OBSERVER:
                    numObservations++;
                    queueSizes.push_back(queue.size());
                    if (queue.empty()) {
                        idleTime += 1;
                    }
                    //observe();
                    break;
            }
            if(!currentPacket || !currentPacket->isServicing()){
                if(!queue.empty()){
                    currentPacket = &queue.front();
                    nextTickDeparture = tick + (currentPacket->getLength()/(transmissionRate*(1000000/totalTicks)));
                    currentPacket->service(nextTickDeparture);
                }
            }
            else if (currentPacket->getDepartureTick() == tick){
                queue.erase(queue.begin());
                numDepartures++;
                currentPacket = nullptr;
            }
        }
        computePerformance();
    } 


    void observe() {
        cout << "Number of Arrivals: " << numArrivals << endl;
        cout << "Number of Departures: " << numDepartures << endl;
        cout << "Number of Observations: " << numObservations << endl;
    }

    void computePerformance() {
        cout << "E[N]:\t" << averageQueueSize() << "\n";
        cout << "P_IDLE:\t" << percentIdle() << "%\n";
    }

    double averageQueueSize() {
        long sum = 0;

        for (int size : queueSizes) {
            sum += size;
        }

        return (double)sum / (double)queueSizes.size();
    }

    double percentIdle() {
        return ((double)idleTime / (double)totalTicks) * 100.0;
    }
};