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
        for (int i = 0; i < weights.size(); i++) {
            std::vector<int> row;
            for (int i = 0; i <= maxWeight; i++) {
                row.push_back(0);
            }
            m.push_back(row);
        }
    }

    std::vector<std::vector<int> > getM();

    std::pair<std::vector<int>, int> solve();
};
