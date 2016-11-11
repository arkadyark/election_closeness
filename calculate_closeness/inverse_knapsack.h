#include<vector>
#include<utility>
#include<iostream>
#include<algorithm>

class InverseKnapsackSolver {
    std::vector<int> weights;
    std::vector<int> values;
    int minValue;

public:
    InverseKnapsackSolver(std::vector<int> weights, std::vector<int> values, int minValue) : weights(weights), values(values), minValue(minValue) {};
    std::vector<int> traceback(std::vector<std::vector<int> >& m, int maxWeight);
    std::pair<std::vector<int>, int> solve();
};
