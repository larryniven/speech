#ifndef UTIL_BATCH_H
#define UTIL_BATCH_H

#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <ios>

namespace batch {

    struct scp {
        struct entry {
            std::string key;
            std::string filename;
            std::streampos shift;
        };

        std::vector<entry> entries;

        std::ifstream ifs_;
        std::shared_ptr<std::string> filename_;

        void open(std::string filename);
        std::istream& at(int i);
    };

    std::vector<double> load_feats(std::istream& is);

    std::string load_label(std::istream& is);

}

#endif
