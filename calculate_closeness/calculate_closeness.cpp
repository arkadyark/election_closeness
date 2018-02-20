#include <fstream>
#include <string>
#include <vector>
#include <cmath>

#include "inverse_knapsack.h"
#include "json.hpp"

using json = nlohmann::json;

int main(int argc, const char *argv[])
{
    // Load data
    std::ifstream ifs("../data/data.json");
    std::string content( (std::istreambuf_iterator<char>(ifs)),
             (std::istreambuf_iterator<char>()    ) );
    json j = json::parse(content);

    // Loop over each election
    for (json::iterator it = j.begin(); it != j.end(); ++it) {
        auto o = *it;
        std::vector<int> weights;
        std::vector<int> values;
        std::vector<std::string> states;
        int totalPossibleVotes;
        int totalVotes;
        int max_votes = 0;
        int second_votes = 0;
        std::string max_candidate = "";
        std::string second_candidate = "";

        // Parse JSON for election, put into format for solver
        for (json::iterator itt = o.begin(); itt != o.end(); ++itt) {
            if (itt.key() == "states") {
                for (json::iterator states_iterator = (*itt).begin(); states_iterator != (*itt).end(); ++states_iterator) {
                    values.push_back(states_iterator.value()[0]);
                    weights.push_back(states_iterator.value()[1]);
                    states.push_back(states_iterator.key());
                }
            }
            if (itt.key() == "year") {
                std::cout << "Year: " << itt.value() << std::endl;
            }
            if (itt.key() == "total") {
                totalVotes = itt.value();
            }
            if (itt.key() == "votes") {
                totalPossibleVotes = 0;
                for (json::iterator votes_iterator = (*itt).begin(); votes_iterator != (*itt).end(); ++votes_iterator) {
                    if ((int) votes_iterator.value() > max_votes) {
                        second_votes = max_votes;
                        second_candidate = max_candidate;
                        max_votes = votes_iterator.value();
                        max_candidate = (std::string) votes_iterator.key();
                    } else if ((int) votes_iterator.value() > second_votes) {
                        second_votes = votes_iterator.value();
                        second_candidate = (std::string) votes_iterator.key();
                    }
                    totalPossibleVotes += (int) votes_iterator.value();
                }
            }
        }

        // Determine fewest votes necessary to flip the election
        int minValue = totalPossibleVotes / 2 + 1 - second_votes;
        std::cout << "Top two candidates were " << max_candidate << " (" << max_votes << ") and " << second_candidate << " (" << second_votes << "). " << totalPossibleVotes / 2 + 1 << " electoral votes were needed to win." << std::endl;

        // Run inverse knapsack problem solver to find how to get the votes
        InverseKnapsackSolver solver(weights, values, minValue);
        std::pair<std::vector<int>, int> optimum = solver.solve();
        std::cout << "" << second_candidate << " needed at least " << minValue << " additional electoral votes" << std::endl;
        std::cout << "Minimum switched votes needed to swing the election: " << optimum.second << std::endl;
        std::cout << "Optimal allocation of those switched votes: " << std::endl;
        for (int i = 0; i < optimum.first.size(); i++) {
            if (optimum.first[i]) {
                std::cout << weights.at(i) << " additional votes in " << states.at(i) << " (" << values.at(i) << ")" << std::endl;
            }
        }

        // Determine how many electoral votes would be gained by optimal allocation
        int actualValue = 0;
        for (int i = 0; i < weights.size(); i++) {
            if (optimum.first.at(i)) {
                actualValue += values[i];
            }
        }
        std::cout << "These added votes would have yielded an additional " << actualValue << " electoral votes" << std::endl;
        std::cout << "The closeness (-ln(fraction of votes that would have to be switched)) of this election is " << -std::log(((double) optimum.second)/totalVotes) << std::endl;
        std::cout << std::endl;
    }
    return 0;
}
