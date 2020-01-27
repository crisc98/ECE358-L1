#include <stdio.h>
#include <stdlib.h>
#include <math.h>

class ExponentialDistribution 
{
    public:
    double generateRandVar(long lambda) {
        double u = (double)rand()/(RAND_MAX-1.0);
        double rv = log(1-u)/(-lambda);
    }
};