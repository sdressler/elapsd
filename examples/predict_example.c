#include "../include/elapsd/celapsd.h"
#include "../include/elapsd/cparams.h"
#include "../include/elapsd/predict/cpredict.h"

#include <stdio.h>
#include <stdlib.h>

unsigned int fib(unsigned int n) {
    if (n < 2) {
        return n;
    } else {
        return fib(n-1) + fib(n-2);
    }
}

int main(int argc, char *argv[]) {
    
    if (argc < 2) {
        printf("FIB not provided!\n");
        return -1;
    }

    int F = strtol(argv[1], NULL, 10);

    /* Create the parameter map */
    elapsdParams *param = elapsdParamsInit();
    elapsdParamsAddParam(param, "FIB", F);

    /* Initialize elaps'd */
    elapsd *e = elapsdInit("elapsd_predict.db", "Fibonacci C", param);

    /* Add Kernel and Device */
    elapsdAddKernel(e, 0, "Fibonacci");
    elapsdAddDevice(e, 0, "CPU");

    /* Initialize runtime predictor for Kernel = 0 and Device = 0 */
    predict *p = predictInit("elapsd_predict.db", param, 0, 0);

    /* TRY to initialize prediction, if no success, continue collecting data */
    int pred_err = predictCreatePredictionModel_Lagrange(p);

    /* Make a prediction, if pred_err == E_OK */
    double FIB_time_pred = -1.0;
    if (pred_err == E_OK) {
        FIB_time_pred = predictGetRuntimePrediction(p, F);
    }

    /* Now, measure the kernel */
    elapsdStartTimer(e, 0, 0);
    int F_result = fib(F);
    elapsdStopTimer(e, 0, 0);
    /* Measurement done. */

    printf("Fibonacci %d -> %u\n\n", F, F_result);

    double FIB_time = elapsdGetLastWallTime(e, 0, 0);

    printf("Predicted runtime : %f\n", FIB_time_pred);
    printf("Real runtime      : %f\n", FIB_time);

    /* Commit to DB */ 
    elapsdCommitToDB(e);

    return 0;

}
