#include "util/batch.h"
#include "ebt/ebt.h"
#include <cassert>
#include <fstream>

namespace batch {

    void scp::open(std::string filename)
    {
        std::ifstream ifs { filename };

        if (!ifs) {
            throw std::logic_error("unable to open " + filename);
        }

        entries.clear();

        std::string line;

        while (std::getline(ifs, line)) {
            auto parts = ebt::split(line);

            entry e;

            e.key = parts[0];

            parts = ebt::split(parts[1], ":");

            e.filename = parts[0];
            e.shift = std::stol(parts[1]);

            entries.push_back(e);
        }
    }

    std::istream& scp::at(int i)
    {
        if (filename_ == nullptr || *filename_ != entries[i].filename) {
            filename_ = std::make_shared<std::string>(entries[i].filename);

            if (ifs_.is_open()) {
                ifs_.close();
            }

            ifs_.open(entries[i].filename);
        }

        ifs_.seekg(entries[i].shift);

        if (ifs_) {
            return ifs_;
        } else {
            throw std::logic_error("seek failed while reading scp");
        }
    }

    std::vector<double> load_feats(std::istream& is)
    {
        std::string line;

        if (!std::getline(is, line)) {
            throw std::logic_error("unable to read istream");
        }

        std::getline(is, line);

        std::vector<double> result;

        auto parts = ebt::split(line);
        for (auto& s: parts) {
            result.push_back(std::stod(s));
        }

        std::getline(is, line);
        assert(line == ".");

        return result;
    }

    std::string load_label(std::istream& is)
    {
        std::string line;

        if (!std::getline(is, line)) {
            throw std::logic_error("unable to read istream");
        }

        std::getline(is, line);

        std::string result = line;

        std::getline(is, line);
        assert(line == ".");

        return result;
    }
}
