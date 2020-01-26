#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Simulator.h"

using namespace std;
int main() {
    cout << "Would you like to specify a value for K? [y/n] ";
    char input;
    cin >> input;
    bool isInfinite = (input == 'y');

    if (isInfinite) {
        cout << "Simulating a M/M/1/K queue." << endl;
    } else {
        cout <<"Simulating a M/M/1 queue." << endl;
    }

    long n = 0;
    long lambda = 0;
    double L = 0.0;
    double C = 0.0;

    cout << "Enter a value for n: ";
    cin >> n;

    cout << "Enter a value for λ: ";
    cin >> lambda;

    cout << "Enter a value for L: ";
    cin >> L;

    cout << "Enter a value for C: ";
    cin >> C;

    if (isInfinite) {
        int K;
        cout << "Enter a value for K: " << endl;
        cin >> K;

        cout << "Starting simulation..." << endl;

        //SimulationK simulation = new SimulationK(lambda, L, C, K);
        //simulation.startSimulation(n);
        //simulation.computePerformance();
    } else {

        cout << "Starting simulation..." << endl;

        Simulator simulator = Simulator(lambda, L, C);
        simulator.startSimulation(n);
        simulator.computePerformance();
    }
}