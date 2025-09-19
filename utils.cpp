#include <iostream>       
#include "utils.h"         

using namespace std;

bool isSubset(const std::vector<int>& subset, const std::vector<int>& set) {
    for (const auto& elem : subset) {
        if (std::find(set.begin(), set.end(), elem) == set.end()) {
            return false;
        }
    }
    return true;
}

std::pair<std::set<char>, std::vector<std::string>> check_constraint(int target_index, const vector<int>& combination, const vector<int>& enumeration, const vector<pair<string, double>>& chromosomes){
    std::set<char> values_at_v;
    std::vector<std::string> highest_fitness_chromosomes;

    double max_fitness = -1;
    for (const auto& chromosome : chromosomes) 
    {
        bool fit_constraint = true;
        int enumeration_index = 0;
        for (int i : combination)
        {
            if (chromosome.first[i] - '0' != enumeration[enumeration_index]){
                fit_constraint = false;
                break;
            }
            enumeration_index++;
        }

        if (fit_constraint) 
        {
            double fitness = chromosome.second;
            if (fitness > max_fitness) {
                max_fitness = fitness;
                values_at_v.clear();
                highest_fitness_chromosomes.clear();
                values_at_v.insert(chromosome.first[target_index]);
                highest_fitness_chromosomes.push_back(chromosome.first);
            } else if (fitness == max_fitness) {
                values_at_v.insert(chromosome.first[target_index]);
                highest_fitness_chromosomes.push_back(chromosome.first);
            } 
        }         
    
    }

    return {values_at_v, highest_fitness_chromosomes};
}

int check_constrained_optima_for_eg(int target_index, const vector<int>& combination, const vector<int>& enumeration_original, const vector<int>& combination_wo, const vector<int>& enumeration_wo, const vector<pair<string, double>>& chromosomes){
    auto constrained_optima_original = check_constraint(target_index, combination, enumeration_original, chromosomes);
    auto constrained_optima_flip = check_constraint(target_index, combination_wo, enumeration_wo, chromosomes);

    if (constrained_optima_original.first != constrained_optima_flip.first) {

        if (!constrained_optima_original.first.empty() && !constrained_optima_flip.first.empty()){

            if (combination.size() == 1 && constrained_optima_original.first.size() == 1 && constrained_optima_flip.first.size() == 1){
                // cout << "{";
                // for (const auto& elem : combination) {
                //     cout << elem << " ";
                // }
                // cout << "} -> "<< target_index << endl; 
                return 1;  
            }else if (combination.size() == 1 && (constrained_optima_original.first.size() != 1 || constrained_optima_flip.first.size() != 1)){
                // cout << "{";
                // for (const auto& elem : combination) {
                //     cout << elem << " ";
                // }
                // cout << "} ->>> "<< target_index << endl;
                return 2;  
            }
        }
        else{
            return 0;
        }
    }else{      
        return 0;
    }

    return 0;
}


bool check_constrained_optima(int target_index, const vector<int>& combination, const vector<int>& enumeration_original, const vector<int>& combination_wo, const vector<int>& enumeration_wo, const vector<pair<string, double>>& chromosomes){
    auto constrained_optima_original = check_constraint(target_index, combination, enumeration_original, chromosomes);
    auto constrained_optima_flip = check_constraint(target_index, combination_wo, enumeration_wo, chromosomes);

    if (constrained_optima_original.first != constrained_optima_flip.first) {

        if (!constrained_optima_original.first.empty() && !constrained_optima_flip.first.empty()){

            if (combination.size() == 1 && constrained_optima_original.first.size() == 1 && constrained_optima_flip.first.size() == 1){
                cout << "{";
                for (const auto& elem : combination) {
                    cout << elem << " ";
                }
                cout << "} -> "<< target_index << endl;   
            }else if (combination.size() == 1 && (constrained_optima_original.first.size() != 1 || constrained_optima_flip.first.size() != 1)){
                cout << "{";
                for (const auto& elem : combination) {
                    cout << elem << " ";
                }
                cout << "} ->>> "<< target_index << endl;
            }

            return true;
        }
        else{
            return false;
        }
    }else{      
        return false;
    }
}

void print_matrix(const vector<vector<string>>& matrix) {
    for (const auto& row : matrix) {
        for (const auto& elem : row) {
            cout << elem << " ";
        }
        cout << endl;
    }
}



