#include "util/util.h"
#include <fstream>

namespace util {

    std::vector<std::string> load_label_set(std::string filename)
    {
        std::vector<std::string> result;

        std::ifstream ifs { filename };
    
        std::string line;
        while (std::getline(ifs, line)) {
            result.push_back(line);
        }
    
        return result;
    }

    std::unordered_map<std::string, int> load_label_id(std::string filename)
    {
        std::unordered_map<std::string, int> result;
        std::string line;
        std::ifstream ifs { filename };

        int i = 0;
        while (std::getline(ifs, line)) {
            result[line] = i;
            ++i;
        }
    
        return result;
    }

}
