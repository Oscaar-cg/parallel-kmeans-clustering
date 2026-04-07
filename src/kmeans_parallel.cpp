#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <omp.h>
#include <chrono>

using namespace std;

int main() {
    srand(1337);

    int n = 1000000; // nombre de points
    int k = 3;
    int maxIterations = 10;

    //Structure of Arrays (SoA)
    vector<float> lat(n), lon(n);
    vector<int> cluster(n, -1);

    vector<float> centroid_x(k), centroid_y(k);

    // Générer données aléatoires
    for (int i = 0; i < n; i++) {
        lat[i] = rand() % 100;
        lon[i] = rand() % 100;
    }

    // Générer centroids
    for (int j = 0; j < k; j++) {
        centroid_x[j] = rand() % 100;
        centroid_y[j] = rand() % 100;
    }

    //START TIMER
    auto start = chrono::high_resolution_clock::now();

    for (int iter = 0; iter < maxIterations; iter++) {

        // ASSIGNATION PARALLÈLE
        //#pragma omp parallel for
        for (int i = 0; i < n; i++) {

            float minDist = 999999;
            int bestCluster = 0;

            for (int j = 0; j < k; j++) {
                float dx = lat[i] - centroid_x[j];
                float dy = lon[i] - centroid_y[j];
                float dist = sqrt(dx*dx + dy*dy);

                if (dist < minDist) {
                    minDist = dist;
                    bestCluster = j;
                }
            }

            cluster[i] = bestCluster;
        }

        //UPDATE CENTROIDS (séquentiel pour éviter bugs)
        vector<float> sumX(k, 0), sumY(k, 0);
        vector<int> count(k, 0);

        for (int i = 0; i < n; i++) {
            int c = cluster[i];
            sumX[c] += lat[i];
            sumY[c] += lon[i];
            count[c]++;
        }

        for (int j = 0; j < k; j++) {
            if (count[j] > 0) {
                centroid_x[j] = sumX[j] / count[j];
                centroid_y[j] = sumY[j] / count[j];
            }
        }
    }

    //END TIMER
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    cout << "Execution time (Sequential SoA): " << duration.count() << " seconds" << endl;

    return 0;
}