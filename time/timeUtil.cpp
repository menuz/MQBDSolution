#include <stdio.h>
#include <unistd.h> // sleep
#include <iostream>
#include <time.h>

using namespace std;
// sleep make time fail 
double getTimeInterval() 
{
    time_t timer1;
    time(&timer1);
    long i = 1000000000;
    while(i--);
    time_t timer2;
    time(&timer2);
    double seconds = difftime(timer2, timer1);
    printf("%.6f seconds\n", seconds);
}

double getTimeInterval2()
{
    clock_t start_time, end_time;
    double elapsed;
    long i = 10000000;
    start_time = clock();
    while(i--);
    end_time = clock();
    elapsed =(double) (end_time - start_time)/CLOCKS_PER_SEC;
    printf("1000000 loop done in %.6f senconds\n", elapsed);
}

int main(int argc, const char *argv[])
{
   getTimeInterval();
   getTimeInterval2();
}


