#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <cmath>
#include <algorithm>
#include <set>
#include <random>
#include "chromosome.h"   
#include "problem.h"     
#include "weak.h"           
#include "utils.h"         

using namespace std;
#define DEBUG 1 // Uncomment this line if DEBUG is meant to be a macro

int check_weak(int target_index, const vector<int>& combination, const vector<vector<int>>& enumerations, const vector<pair<string, double>>& chromosomes) {

    // cout << "==================" << endl;
    // cout << "combination: ";
    // for (const auto& elem : combination) {
    //     cout << elem << " ";
    // }
    // cout << endl;
    

    // auto enumeration_original = enumeration;
    int condition_holds = 0;
    for (int condition_index = 0; condition_index < combination.size(); condition_index++)
    {
        for (auto& enumeration : enumerations) // combination = [1, 2] and enumerations = { [0, 0], [0, 1], [1, 0], [1, 1]}
        { 
            auto enumeration_original = enumeration;
            // enumeration[condition_index] = 1 - enumeration[condition_index]; // flip bit  

            auto combination_wo = combination;
            auto enumeration_wo = enumeration;
            combination_wo.erase(combination_wo.begin() + condition_index);
            enumeration_wo.erase(enumeration_wo.begin() + condition_index);

            // cout << "combination: ";
            // for (const auto& elem : combination) {
            //     cout << elem << " ";
            // }   
            // cout<<endl;

            // cout << "combination_wo: ";
            // for (const auto& elem : combination_wo) {
            //     cout << elem << " ";
            // }
            // cout<<endl;

            // cout << "enumeration: ";
            // for (const auto& elem : enumeration) {
            //     cout << elem << " ";
            // }
            // cout<<endl;

            // cout << "enumeration_wo: ";
            // for (const auto& elem : enumeration_wo) {
            //     cout << elem << " ";
            // }
            // cout<<endl;
            
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
            if (DEBUG and combination.size() > 1){  
                cout << "{";
                for (const auto& elem : combination) {
                    cout << elem << " ";
                }
                cout << "} -> "<< target_index << endl;           
            }

            // if (DEBUG){
            //     cout << "--------------------------------------" << endl;
            //     cout << "enumeration_original" << endl;
            //     for (const auto& elem : enumeration_original) {
            //         cout << elem << " ";
            //     }
            //     cout << endl;          
            // }
            return 1;
        }


    // if (DEBUG){
    //     cout << endl;
    //     cout << "---------- in check_weak --------------" << endl;
    // }


    // if (DEBUG)
    // {
    //     cout << "current combination index is ";
    //     for (const auto& elem : combination) {
    //         cout << elem << " ";
    //     }
    //     cout << endl;
    // }
    
    
    
    // if (DEBUG)
    // {
    //     cout << "current enumeration is ";
    //     for (const auto& elem : enumeration) {
    //         cout << elem << " ";
    //     }
    //     cout << endl;
    // }

    // cout << "current combination index is ";
    // for (const auto& elem : combination) {
    //     cout << elem << " ";
    // }
    // cout << endl;

    // cout << "current enumeration is ";
    // for (const auto& elem : enumeration) {
    //     cout << elem << " ";
    // }
    // cout << "target_index = "<< target_index <<endl;


    // auto enumeration_original = enumeration;
    // int condition_holds = 0;
    // for (int condition_index = 0; condition_index < enumeration.size(); condition_index++)
    // {
    //     // cout << "--------------------------------------" << endl;
    //     // cout << "enumeration_original" << endl;
    //     // for (const auto& elem : enumeration_original) {
    //     //     cout << elem << " ";
    //     // }
    //     // cout << endl;

    //     enumeration = enumeration_original;
    //     enumeration[condition_index] = 1 - enumeration[condition_index]; // flip bit 

    //     // if (DEBUG)
    //     // {
    //     //     cout << "A =  ";
    //     //     for (const auto& elem : enumeration_original) {
    //     //         cout << elem << " ";
    //     //     }
    //     //     cout << endl;
    //     // }

    //     // if (DEBUG)
    //     // {
    //     //     cout << "B =  ";
    //     //     for (const auto& elem : enumeration) {
    //     //         cout << elem << " ";
    //     //     }
    //     //     cout << endl;
    //     // }
        
    //     bool not_equal;
    //     not_equal = check_constrained_optima(target_index, combination, enumeration_original, enumeration, chromosomes);
    //     // cout << "not_equal: " << not_equal << endl;
    //     // cout << "######################################" << endl;

    //     if (not_equal)
    //     {
    //         condition_holds++;
    //     }else{
    //         return 0;
    //     }
    // }

    // if (condition_holds == enumeration.size())
    // {
    //     if (DEBUG){  
    //         cout << "{";
    //         for (const auto& elem : combination) {
    //             cout << elem << " ";
    //         }
    //         cout << "} -> "<< target_index << endl;           
    //     }

        // if (DEBUG){
        //     cout << "--------------------------------------" << endl;
        //     cout << "enumeration_original" << endl;
        //     for (const auto& elem : enumeration_original) {
        //         cout << elem << " ";
        //     }
        //     cout << endl;          
        // }
    //     return 1;
    // }

    // if (DEBUG){
    //     cout << "---------- out check_weak --------------" << endl;
    // }

    return 0;
}


std::vector<int> weak(int L, int target_index, const vector<pair<string, double>>& chromosomes)
{
    std::vector<std::vector<std::vector<int>>> epi_set(L);
    std::vector<int> epi_count(L, 0); 
    
    for (int epi_size = 1; epi_size < L; epi_size++)
    {  
        auto combinations = generateCombinations(L-1, epi_size, target_index); // combinations = { [1, 2], [1, 3], [2, 3] }

        for (auto& combination : combinations) // combination = [1, 2]
        { 

            bool find_smaller_epi;

            if (epi_size == 1)
            {
                find_smaller_epi = false;           
            }else{
                find_smaller_epi = false;  
                int smaller_epi_size = epi_size;
                bool is_subset;
                while(!find_smaller_epi && smaller_epi_size>=1)
                {   
                    for (auto& previous : epi_set[smaller_epi_size-1]) 
                    {
                        is_subset = isSubset(previous, combination);
                        find_smaller_epi = is_subset;
                        if(is_subset) break;
                    }
                    smaller_epi_size--;
                }
            }

            if(!find_smaller_epi)
            {
                auto enumerations = generateBinarySequences(epi_size); // enumerations = { [0, 0], [0, 1], [1, 0], [1, 1] }    

                int result = check_weak(target_index, combination, enumerations, chromosomes);
                epi_count[epi_size] += result;
                if (result)
                {
                    epi_set[epi_size].push_back(combination);
                }
            
            
            }
        }
    }

    return epi_count;
}


