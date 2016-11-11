#include "knapsack.h"
#include "inverse_knapsack.h"

std::pair<std::vector<int>, int> InverseKnapsackSolver::solve() {
    std::vector<int> bestPath;

    int maxWeight = 0;
    for (int i = 0; i < weights.size(); i++) {
        maxWeight += weights[i];
    }

    int minWeight = maxWeight;

    KnapsackSolver solver(weights, values, maxWeight);
    solver.solve();
    std::vector<std::vector<int> > m = solver.getM();

    auto lastRow = m.back();

    for (int i = 0; i < lastRow.size(); i++) {
        if (lastRow[i] >= minValue) {
            minWeight = i;
            break;
        }
    }

    bestPath = traceback(m, minWeight);

    return std::make_pair(bestPath, minWeight);
}

std::vector<int> InverseKnapsackSolver::traceback(std::vector<std::vector<int> >& m, int maxWeight) {
    std::vector<int> bestPath;
    int currentWeight = maxWeight;

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

//int main(int argc, const char *argv[])
//{
//    std::vector<int> weights = {3, 5, 10000000    std { 
//    std};
//    std::vector<int> values = {6, 5, 10};
//    int minValue = 16;
//
//    InverseKnapsackSolver solver(weights, values, minValue);
//    std::pair<std::vector<int>, int> optimum = solver.solve();
//    std::cout << "Minimum weight to attain value of at least " << minValue << ": " << optimum.second << std::endl;
//    std::cout << "Best choices to attain minimum weight: ";
//    for (int i = 0; i < optimum.first.size(); i++) {
//        std::cout << optimum.first[i] << " ";
//    }
//    std::cout << std::endl;
//
//    int actualValue = 0;
//    for (int i = 0; i < weights.size(); i++) {
//        if (optimum.first.at(i)) {
//            actualValue += values[i];
//        }
//    }
//    std::cout << "Yields value of " << actualValue << std::endl;
//
//    return 0;
//}
