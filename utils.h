#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <string>
#include <set>
#include <utility>

bool isSubset(
    const std::vector<int>& subset,
    const std::vector<int>& set);

std::pair<std::set<char>, std::vector<std::string>> check_constraint(
    int target_index,
    const std::vector<int>& combination,
    const std::vector<int>& enumeration,
    const std::vector<std::pair<std::string, double>>& chromosomes);

bool check_constrained_optima(
    int target_index,
    const std::vector<int>& combination,
    const std::vector<int>& enumeration_original,
    const std::vector<int>& combination_wo,
    const std::vector<int>& enumeration_wo,
    const std::vector<std::pair<std::string, double>>& chromosomes);

#endif 
