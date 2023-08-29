/******************************************************************************
 * File: Utilities.h
 * Description: Holds helper functions for the other files in the project.
 * Author: Robert Tetreault
 ******************************************************************************/

#ifndef UTILITIES_H
#define UTILITIES_H


#include <vector>
#include <unordered_map>

class Utilities {
public:
    template <typename Key, typename Value>
    static std::vector<Value> mapToVector(const std::unordered_map<Key, Value>& map) {
        std::vector<Value> vec;
        for (const auto& pair : map) {
            vec.push_back(pair.second);
        }
        return vec;
    }
};

#endif