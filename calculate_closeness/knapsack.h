#include<vector>
#include<iostream>
#include<utility>
#include <stdexcept>

class KnapsackSolver {
    std::vector<int> weights;
    std::vector<int> values;
    std::vector<std::vector<int> > m;
    int maxWeight;

public:
    KnapsackSolver(std::vector<int>& weights, std::vector<int>& values, int maxWeight) : weights(weights), values(values), maxWeight(maxWeight) {
        // Create the matrix M to hold the solutions
        for (int i = 0; i < weights.size(); i++) {
            std::vector<int> row;
            for (int i = 0; i <= maxWeight; i++) {
                row.push_back(0);
            }
            m.push_back(row);
        }
    }

    // Get the matrix M produced as a byproduct of solving
    std::vector<std::vector<int> > getM();

    // Solve the given knapsack problem
    int solve();
};
