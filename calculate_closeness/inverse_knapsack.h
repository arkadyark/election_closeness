#include<vector>
#include<utility>
#include<iostream>
#include<algorithm>

class InverseKnapsackSolver {
    std::vector<int> weights; // Flipped states
    std::vector<int> values; // Number of electoral college votes each state is worth
    int minValue; // Number of electoral college votes needed to flip result

public:
    InverseKnapsackSolver(std::vector<int> weights, std::vector<int> values, int minValue) : weights(weights), values(values), minValue(minValue) {};
    // Find minimum weight needed to get at least minValue value
    std::pair<std::vector<int>, int> solve();
    // Traceback to find the actual set of weights that attains this
    std::vector<int> traceback(std::vector<std::vector<int> >& m, int maxWeight);
};
