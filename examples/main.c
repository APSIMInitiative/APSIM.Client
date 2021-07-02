#include <stdio.h>
#include <stdlib.h>

// This should be moved eventually.
#include <assert.h>

// tmp
#include <string.h>
#include <time.h>

#include "replacement.h"
#include "client.h"

int main(int argc, char** argv)
{
    // This is the name of the pipe as defined in the C# server code.
    char *pipeName = "testpipe";

    // Connect to the socket.
    printf("Connecting to server...");
    fflush(stdout);
    int sock = connectToServer(pipeName);
    printf("connected\n");

    // OK, let's try and kick off a simulation run.
    // We will be mofifying the juvenile TT target.
    char* path = "[Sorghum].Phenology.Juvenile.Target.FixedValue";
    double value = 120.5;
    for (int iter = 0; iter < 100; iter++) {
        struct Replacement* change = createDoubleReplacement(path, value);
        printf("Running sims with the following changes:\n");
        printf("  %s = %.2f\n", path, value);
        runWithChanges(sock, &change, 1);
        replacement_free(change);

        // Let's read some outputs.
        char* table = "HarvestReport";
        uint32_t nparams = 1;
        char* params[nparams];
        params[0] = "BiomassWt";
        // params[1] = "Sorghum.AboveGround.Wt";
        // params[2] = "Sorghum.Leaf.LAI";
        printf(" ");
        clock_t t = clock();

        struct output** outputs = readOutput(sock, table, params, nparams);
        t = clock() - t;
        double duration = 1000 * (double)t / CLOCKS_PER_SEC;
        printf("Read %d outputs in %.2fms\n", nparams, duration);
        for (uint32_t i = 0; i < nparams; i++) {
            uint32_t n = outputs[i]->len / sizeof(double); // all of our outputs are double for now
            printf("Received output %s with %u elements: [", params[i], n);
            for (size_t j = 0; j < n; j++) {
                double val;
                memcpy(&val, &outputs[i]->data[j * sizeof(double)], sizeof(double));
                printf("%.2f%s", val, j < n - 1 ? ", " : "");
            }
            free(outputs[i]->data);
            free(outputs[i]);
            printf("]\n\n");
        }
        free(outputs);
        value += 5;
    }
    // Close the socket connection.
    printf("Disconnecting from server...\n");
    disconnectFromServer(sock);
    return 0;
}
