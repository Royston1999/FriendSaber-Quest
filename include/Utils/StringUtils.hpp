#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <vector>
#include "UnityEngine/Color.hpp"

namespace FriendSaber::Utils{
    std::string toLower(std::string string);
    std::vector<std::string> split(const std::string& s, char delim);
    std::string setToString(std::unordered_set<std::string> set);
}