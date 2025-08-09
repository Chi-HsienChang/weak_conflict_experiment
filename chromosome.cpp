#include <iostream>
#include <bitset>
#include <random>
#include "problem.h"  
#include "chromosome.h"  

using namespace std;

vector<pair<string, double>> generate_chromosomes(int ell, const string& method) {
    cout << method << " with ell = " << ell << endl;

    vector<pair<string, double>> chromosomes;
    int num_combinations = pow(2, ell);

    for (int i = 0; i < num_combinations; ++i) {
        string chromosome = bitset<32>(i).to_string().substr(32 - ell);
        double fitness = calculate_fitness(chromosome, method);
        chromosomes.push_back({chromosome, fitness});
    }
    return chromosomes;
}

std::vector<std::vector<int>> generateBinarySequences(int n) {
    int totalSequences = 1 << n;  
    std::vector<std::vector<int>> allSequences;

    for (int i = 0; i < totalSequences; ++i) {
        std::vector<int> sequence(n);
        for (int j = 0; j < n; ++j) {
            sequence[n - 1 - j] = (i >> j) & 1;
        }
        allSequences.push_back(sequence);
    }
    return allSequences;
}

std::vector<std::vector<int>> generateCombinations(int n, int k, int target_index) {
    std::vector<int> elements;

    for (int i = 0; i <= n; ++i) { 
        if (i != target_index) {
            elements.push_back(i);
        }
    }

    std::vector<int> bitmask(k, 1);           
    bitmask.resize(elements.size(), 0);      

    std::vector<std::vector<int>> combinations;

    do {
        std::vector<int> currentCombination;
        for (size_t i = 0; i < elements.size(); ++i) {
            if (bitmask[i]) {
                currentCombination.push_back(elements[i]);
            }
        }
        combinations.push_back(currentCombination);
    } while (std::prev_permutation(bitmask.begin(), bitmask.end())); 

    return combinations;
}

vector<pair<string, double>> sample_chromosomes(const vector<pair<string, double>>& all_chromosomes, int n) {
   
    vector<pair<string, double>> sampled_chromosomes = all_chromosomes;
    random_device rd;
    mt19937 gen(rd());
    shuffle(sampled_chromosomes.begin(), sampled_chromosomes.end(), gen);

    if (n < sampled_chromosomes.size()) {
        sampled_chromosomes.resize(n);
    }

    return sampled_chromosomes;
}
