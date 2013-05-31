#include "../include/elapsd/celapsd.h"

#include <stdio.h>
#include <stdlib.h>

unsigned int fib(unsigned int n) {
    if (n < 2) {
        return n;
    } else {
        return fib(n-1) + fib(n-2);
    }
}

int main(void) {

    unsigned int f;
    int i, err;

    elapsd *e = elapsdInit("elapsd.db", "Fibonacci C");

    printf("Adding kernel.\n");
    err = elapsdAddKernel(e, 0, "Fibonacci");

    // Do not forget the ERRORS!
    if (err != E_OK) {
        printf("Error: %d\n", err);
        exit(-1);
    }

    printf("Adding device\n");
    elapsdAddDevice(e, 0, "CPU");

    printf("Generating timings and datavolumes\n");

    for (i = 0; i < 4; i++) {

        // Fib number to generate
        f = (i + 1) * 10;

        // Start Timer
        elapsdStartTimer(e, 0, 0);
        
        // Run Kernel
        f = fib(f);

        // Stop Timer
        elapsdStopTimer(e, 0, 0);

        // Set KDV
        elapsdAddKernelDataVolumes(e, 0, 0, 4, 4);

        printf("Fibonacci %d: %u\n", (i+1)*10, f);
    }

    printf("Printing content:\n");
    elapsdPrintResults(e);

    printf("Committing to DB\n");
    elapsdCommitToDB(e);

    return 0;

}
