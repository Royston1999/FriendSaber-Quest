#include "Utils/StringUtils.hpp"

namespace FriendSaber::Utils{

    std::string toLower(std::string s){
        for (char& c : s) c = std::tolower(c);
        return s;
    }

    std::vector<std::string> split(const std::string& s, char delim)
    {
        std::stringstream ss(s);
        std::string item;
        std::vector<std::string> elems;
        while (std::getline(ss, item, delim)) elems.push_back(item);
        return elems;
    }

    std::string setToString(std::unordered_set<std::string> set){
        std::ostringstream stream;
        std::copy(set.begin(), set.end(), std::ostream_iterator<std::string>(stream, ","));
        std::string result = stream.str().substr(0, stream.str().size()-1);
        return result;
    }
}