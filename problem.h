#ifndef PROBLEM_H
#define PROBLEM_H

#include <string>

double calculate_segment_onemax_weak(const std::string& segment, const std::string& method);

double calculate_segment_fitness(const std::string& segment, const std::string& method);

double calculate_segment_fitness_test(const std::string& segment);

double calculate_onemax_weak(const std::string& segment, const std::string& method);

double calculate_fitness(const std::string& chromosome, const std::string& method);

#endif 
