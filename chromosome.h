#ifndef CHROMOSOME_H
#define CHROMOSOME_H

#include <vector>
#include <string>
#include <utility>

std::vector<std::pair<std::string, double>> generate_chromosomes(int L, const std::string& method);

std::vector<std::vector<int>> generateBinarySequences(int n);

std::vector<std::vector<int>> generateCombinations(int n, int k, int target_index);

std::vector<std::pair<std::string, double>> sample_chromosomes(
    const std::vector<std::pair<std::string, double>>& all_chromosomes,
    int n
);

#endif 
