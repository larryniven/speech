#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include "ebt/ebt.h"

int main(int argc, char *argv[])
{
    ebt::ArgumentSpec spec {
        "shuffle-frames",
        "Shuffle frames",
        {
            {"frames", "", true}
        }
    };

    if (argc == 1) {
        ebt::usage(spec);
        exit(1);
    }

    std::unordered_map<std::string, std::string> args
        = ebt::parse_args(argc, argv, spec);

    std::vector<std::vector<double>> frames;

    std::ifstream ifs { args.at("frames") };
    std::string line;

    while (std::getline(ifs, line)) {
        std::vector<std::string> parts = ebt::split(line);

        std::vector<double> v;
        for (auto& e: parts) {
            v.push_back(std::stod(e));
        }

        frames.push_back(v);
    }

    std::shuffle(frames.begin(), frames.end(), std::default_random_engine());

    for (auto& f: frames) {
        std::cout << ebt::join(f, " ") << std::endl;
    }

    return 0;
}
