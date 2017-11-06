#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <string>
#include <unordered_map>

namespace util {

    std::vector<std::string> load_label_set(std::string filename);

    std::unordered_map<std::string, int> load_label_id(std::string filename);
}

#endif
