#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <omp.h>
#include <chrono>

using namespace std;

// Point
struct Point {
    float x, y;
    int cluster;
};

// Centroid
struct Centroid {
    float x, y;
};

// Générer points
vector<Point> generatePoints(int n) {
    vector<Point> points;

    for (int i = 0; i < n; i++) {
        Point p;
        p.x = rand() % 100;
        p.y = rand() % 100;
        p.cluster = -1;
        points.push_back(p);
    }

    return points;
}

// Générer centroids
vector<Centroid> generateCentroids(int k) {
    vector<Centroid> centroids;

    for (int i = 0; i < k; i++) {
        Centroid c;
        c.x = rand() % 100;
        c.y = rand() % 100;
        centroids.push_back(c);
    }

    return centroids;
}

// Distance euclidienne
float distance(Point p, Centroid c) {
    return sqrt(pow(p.x - c.x, 2) + pow(p.y - c.y, 2));
}

int main() {
    srand(time(0));

    int numPoints = 100000; // augmente pour voir la diff
    int k = 2;
    int maxIterations = 10;

    vector<Point> points = generatePoints(numPoints);
    vector<Centroid> centroids = generateCentroids(k);

    // 🔥 START TIMER (ICI)
    auto start = chrono::high_resolution_clock::now();

    for (int iter = 0; iter < maxIterations; iter++) {

        // 🔹 1. ASSIGNATION (PARALLELE)
        //#pragma omp parallel for
        for (int i = 0; i < (int)points.size(); i++) {
            float minDist = 999999;
            int bestCluster = 0;

            for (int j = 0; j < (int)centroids.size(); j++) {
                float dist = distance(points[i], centroids[j]);

                if (dist < minDist) {
                    minDist = dist;
                    bestCluster = j;
                }
            }

            points[i].cluster = bestCluster;
        }

        // 🔹 2. RECALCUL DES CENTROIDS
        vector<float> sumX(k, 0);
        vector<float> sumY(k, 0);
        vector<int> count(k, 0);

        for (auto &p : points) {
            sumX[p.cluster] += p.x;
            sumY[p.cluster] += p.y;
            count[p.cluster]++;
        }

        for (int j = 0; j < k; j++) {
            if (count[j] > 0) {
                centroids[j].x = sumX[j] / count[j];
                centroids[j].y = sumY[j] / count[j];
            }
        }
    }

    // 🔥 END TIMER (ICI)
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> duration = end - start;
    cout << "Execution time: " << duration.count() << " seconds" << endl;

    return 0;
}