#ifndef WEAK_H
#define WEAK_H

#include <vector>
#include <string>
#include <set>
#include <utility>

// Check if 'subset' elements all exist in 'set'
bool isSubset(const std::vector<int>& subset, const std::vector<int>& set);

// Evaluate constraint for a given combination and enumeration over chromosomes
// Returns a pair of:
//  - set<char> of target locus values at optimum
//  - vector<string> of highest-fitness chromosomes satisfying the constraint
std::pair<std::set<char>, std::vector<std::string>> 
check_constraint(
    int target_index,
    const std::vector<int>& combination,
    const std::vector<int>& enumeration,
    const std::vector<std::pair<std::string, double>>& chromosomes
);

// Compare constraint optima with and without one locus to detect change
bool check_constrained_optima(
    int target_index,
    const std::vector<int>& combination,
    const std::vector<int>& enumeration_original,
    const std::vector<int>& combination_wo,
    const std::vector<int>& enumeration_wo,
    const std::vector<std::pair<std::string, double>>& chromosomes
);

// Check epistatic effect for a single combination of loci
// Returns 1 if epistasis detected, otherwise 0
int check_weak(
    int target_index,
    const std::vector<int>& combination,
    const std::vector<std::vector<int>>& enumerations,
    const std::vector<std::pair<std::string, double>>& chromosomes
);

// Compute epistasis counts for all combination sizes up to L-1
std::vector<int> weak(
    int L,
    int target_index,
    const std::vector<std::pair<std::string, double>>& chromosomes
);

// Print a matrix of strings to standard output
void print_matrix(const std::vector<std::vector<std::string>>& matrix);

#endif // WEAK_H
