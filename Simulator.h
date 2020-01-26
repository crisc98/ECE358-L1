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
    long packetsPerSecond;
    long serviceTime;
    long totalTicks = 0;
    long nextTick = 0;
    long idleTime = 0;

    public:
    Simulator(long pps, double L, double rate) 
    {
        packetsPerSecond = pps;
        serviceTime = (long)((L/rate) * 1000000.0);
    }
    void startSimulation(long ticks) {
        totalTicks = ticks;

        for (long tick = 0; tick < ticks; tick++) {
            if (arrival(tick)) {
                //cout << "Arrival" << endl;
                queueSizes.push_back(queue.size());
            }

            if (departure(tick)) {
                cout << "Departure" << endl;
                queueSizes.push_back(queue.size());
            }
        }
    } 
    bool arrival(long tick) {
        if (tick >= nextTick) {
            queue.push_back(Packet(tick));
            ExponentialDistribution e;
            double randomVariable = e.generateRandVar(packetsPerSecond);
            nextTick = tick + (long)(randomVariable * 100);

            return true;
        }

        return false;
    }

    bool departure(long tick) {
        if (queue.empty()) {
            idleTime += 1;
            return false;
        }

        Packet packet = queue.front();

        if (!packet.isServicing()) {
            //cout << "Servicing Packet" << endl;
            packet.service(tick);
        }

        for(int i=0; i< queue.size(); i++){
            if (tick == queue[i].getDepartureTick() + serviceTime) {
                cout << "Erased";
                queue.erase(queue.begin()+i);
                return true;
            }
        }

        return false;
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