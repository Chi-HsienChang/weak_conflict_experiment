#include <iostream>
#include "chromosome.h"   
#include "eg.h"        

using namespace std;
#define DEBUG 0

int check_epistasis(int target_index, const vector<int>& combination, const vector<vector<int>>& enumerations, const vector<pair<string, double>>& chromosomes) {

    if (DEBUG) {
        cout << "==================" << endl;
        cout << "combination_locus: ";
        for (const auto& elem : combination) {
            cout << elem << " ";
        }
        cout << endl;
    }
  
 
    int condition_holds = 0;
    for (int condition_index = 0; condition_index < combination.size(); condition_index++)
    {
        for (auto& enumeration : enumerations) // combination_locus = [1, 2] and enumeration_allele = { [0, 0], [0, 1], [1, 0], [1, 1]}
        { 
            auto enumeration_original = enumeration;
        
            auto combination_wo = combination;
            auto enumeration_wo = enumeration;
            combination_wo.erase(combination_wo.begin() + condition_index);
            enumeration_wo.erase(enumeration_wo.begin() + condition_index);

            if (DEBUG) {
                cout << "enumeration_allele: ";
                for (const auto& elem : enumeration) {
                    cout << elem << " ";
                }
                cout << endl;

                cout << "enumeration_allele_wo: ";
                for (const auto& elem : enumeration_wo) {
                    cout << elem << " ";
                }
                cout << endl;
            }
      
            bool not_equal;
            not_equal = check_constrained_optima(target_index, combination, enumeration_original, combination_wo, enumeration_wo, chromosomes);

            if (not_equal)
            {
                condition_holds++;
                break;
            }       
        }

    }

    if (condition_holds == combination.size())
        {
            if (combination.size() >= 2){
                cout << "{";
                for (const auto& elem : combination) {
                    cout << elem << " ";
                }
                cout << "} -> "<< target_index << endl;           
            }
            return 1;
        }

    return 0;
}


int check_eg(int target_index, const vector<int>& combination, const vector<vector<int>>& enumerations, const vector<pair<string, double>>& chromosomes) {

    int eg_type;
    // int condition_holds = 0;
    for (int condition_index = 0; condition_index < combination.size(); condition_index++)
    {
        for (auto& enumeration : enumerations) // combination_locus = [1, 2] and enumeration_allele = { [0, 0], [0, 1], [1, 0], [1, 1]}
        { 
            auto enumeration_original = enumeration;
        
            auto combination_wo = combination;
            auto enumeration_wo = enumeration;
            combination_wo.erase(combination_wo.begin() + condition_index);
            enumeration_wo.erase(enumeration_wo.begin() + condition_index);
      
            
            eg_type = check_constrained_optima_for_eg(target_index, combination, enumeration_original, combination_wo, enumeration_wo, chromosomes);

            if (eg_type != 0)
            {
                // condition_holds++;
                break;
            }       
        }

    }

    return eg_type;
}



std::vector<int> eg(int L, int target_index, const vector<pair<string, double>>& chromosomes)
{
    std::vector<std::vector<std::vector<int>>> epi_set(L);
    std::vector<int> epi_count(L, 0); 

    std::vector<std::vector<int>> combinations;
    std::vector<std::vector<int>> enumerations;

    for (int i = 0; i < L; i++) {
        combinations.push_back({i});
    }


    for (int i = 0; i < 2; i++) {
        enumerations.push_back({i});
    }
    

    int counter = 0;
    for (auto& combination : combinations) 
    { 
        epi_count[counter] += check_eg(target_index, combination, enumerations, chromosomes);     
        // cout << "counter is " << counter << ": " << epi_count[counter] << endl;
        counter++;  
    }

    return epi_count;
}

std::vector<int> epistasis(int L, int target_index, const vector<pair<string, double>>& chromosomes)
{
    std::vector<std::vector<std::vector<int>>> epi_set(L);
    std::vector<int> epi_count(L, 0); 
    std::vector<std::vector<int>> combinations;
    std::vector<std::vector<int>> enumerations;
    
    for (int epi_size = 1; epi_size < L; epi_size++)
    {  
        combinations = generateCombinations(L-1, epi_size, target_index); // combinations = { [1, 2], [1, 3], [2, 3] }

        for (auto& combination : combinations) // combination = [1, 2]
        { 
            enumerations = generateBinarySequences(epi_size); // enumerations = { [0, 0], [0, 1], [1, 0], [1, 1] }    
            epi_count[epi_size] += check_epistasis(target_index, combination, enumerations, chromosomes);         
        }
    }
    return epi_count;
}



