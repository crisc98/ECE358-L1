#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() 
{
    int i, n;
    double rv, u, lambda, mean;
    mean = 0;
    lambda = 75;
    n = 1000;
   
    /* Intializes random number generator */
    srand((unsigned) time(0));

    /* Print 5 random numbers from 0 to 99 */
    for( i = 0 ; i < n ; i++ ) {
        u = (double)rand()/(RAND_MAX-1.0);
        rv = log(1-u)/(-lambda);
        mean += rv;
        //printf("%.8f\t%.8f\t%.8f\t\n", u, log(1-u), rv);
    }
    mean = mean/n;
    printf("%.8f\n", mean);
   
    return(0);
}
