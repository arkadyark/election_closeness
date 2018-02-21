#include "knapsack.h"

std::vector<std::vector<int> > KnapsackSolver::getM() {
    return m;
}

int KnapsackSolver::solve() {
    // Initialize the first row of the matrix
    for (int i = 0; i <= maxWeight; i++) {
        if (weights[0] <= i) {
            m[0][i] = values[0];
        }
    }

    // Fill the dynamic programming matrix
    for (int i = 1; i < weights.size(); i++) {
        for (int j = 0; j <= maxWeight; j++) {
            if (weights[i] > j) {
                m[i][j] = m[i - 1][j];
            } else {
                try {
                    m[i][j] = std::max(m[i - 1][j], m[i - 1].at(j - weights[i]) + values[i]);
                } catch (const std::out_of_range& oor) {}
            }
        }
    }

    // Compute the max value of weight at most maxWeight
    return m[weights.size() - 1][maxWeight - 1];
}

