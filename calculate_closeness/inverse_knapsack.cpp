#include "knapsack.h"
#include "inverse_knapsack.h"

std::pair<std::vector<int>, int> InverseKnapsackSolver::solve() {
    // Solve knapsack problem with a knapsack size of all of the votes
    // Byproduct of this is the matrix M, with the optimal solutions to the subproblems as well
    int maxWeight = 0;
    for (int i = 0; i < weights.size(); i++) {
        maxWeight += weights[i];
    }
    KnapsackSolver solver(weights, values, maxWeight);
    solver.solve();
    std::vector<std::vector<int> > m = solver.getM();

    // Find the minimum weight that yields at least minValue
    std::vector<int> lastRow = m.back();
    int minWeight = maxWeight;
    for (int i = 0; i < lastRow.size(); i++) {
        if (lastRow[i] >= minValue) {
            minWeight = i;
            break;
        }
    }

    // Traceback to get the actual path corresponding to the weight minWeight
    std::vector<int> bestPath = traceback(m, minWeight);

    return std::make_pair(bestPath, minWeight);
}

std::vector<int> InverseKnapsackSolver::traceback(std::vector<std::vector<int> >& m, int maxWeight) {
    // Find the path through the matrix M corresponding to the weight
    std::vector<int> bestPath;
    int currentWeight = maxWeight;

    // Determine whether state i is included in the best allocation of votes
    for (int i = weights.size(); i > 1; i--) {
        if (currentWeight >= weights[i-1] && m.at(i-2).at(currentWeight - weights[i-1]) == m.at(i-1).at(currentWeight) - values[i-1]) {
            bestPath.insert(bestPath.begin(), 1);
            currentWeight -= weights[i-1];
        } else {
            bestPath.insert(bestPath.begin(), 0);
        }
    }
    if (currentWeight >= weights[0]) {
        bestPath.insert(bestPath.begin(), 1);
    } else {
        bestPath.insert(bestPath.begin(), 0);
    }

    return bestPath;
}
