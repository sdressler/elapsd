#include <elapsd/elapsd.h>
#include <elapsd/predict/predict.h>

/* Prediction model to be used */
#include <elapsd/predict/LagrangePredictionModel.h>

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
    ENHANCE::elapsdParams params;
    params["FIB"] = F;

    /* Initialize elaps'd */
    ENHANCE::elapsd e("elapsd_predict_cpp.db", "Fibonacci C++ Prediction", params);

    /* Add Kernel and Device */
    e.addKernel(0, "Fibonacci");
    e.addDevice(0, "CPU");

    /* Initialize runtime predictor for Kernel = 0 and Device = 0 */
    ENHANCE::predict p("elapsd_predict_cpp.db", params.getKeys(), 0, 0);
    
    /* TRY to initialize prediction, if no success, continue collecting data */
    bool model_ready = true;
    try {
        p.createPredictionModel<ENHANCE::LagrangePredictionModel>();
    } catch (std::invalid_argument &e) {
        model_ready = false;
    }

    /* Make a prediction, if pred_err == E_OK */
    double FIB_time_pred = -1.0;
    if (model_ready) {
        FIB_time_pred = p.getRuntimePrediction(F);
    }

    /* Now, measure the kernel */
    e.startTimer(0, 0);
    int F_result = fib(F);
    e.stopTimer(0, 0);
    /* Measurement done. */

    std::cout << "Fibonacci " << F << " -> " << F_result << "\n\n";

    double FIB_time = e.getLastWallTime(0, 0);

    std::cout << "Predicted runtime : " << FIB_time_pred << "\n";
    std::cout << "Real runtime      : " << FIB_time << "\n";

    /* Commit to DB */
    e.commitToDB();

    return 0;

}
