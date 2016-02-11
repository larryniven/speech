#include <algorithm>
#include <random>
#include <fstream>
#include "ebt/ebt.h"
#include "speech/speech.h"

int main(int argc, char *argv[])
{
    ebt::ArgumentSpec spec {
        "gen343",
        "Generate segments in 3-4-3 features",
        {
            {"acoustic-list", "", true},
            {"segment-list", "", true},
            {"phone-set", "", true},
            {"label-output", "", true},
            {"feature-output", "", true},
        }
    };

    if (argc == 1) {
        ebt::usage(spec);
        exit(1);
    }

    auto args = ebt::parse_args(argc, argv, spec);

    std::cout << args << std::endl;

    std::ifstream acoustic_list { args.at("acoustic-list") };
    std::ifstream segment_list { args.at("segment-list") };
    std::vector<std::string> phone_vec = speech::load_label_set(args.at("phone-set"));
    std::unordered_set<std::string> phone_set { phone_vec.begin(), phone_vec.end() };

    std::unordered_map<std::string, int> phone_id;
    int i = 0;
    for (auto& p: phone_set) {
        phone_id[p] = i;
        ++i;
    }

    std::ofstream label_file { args.at("label-output") };
    std::ofstream output_file { args.at("feature-output") };

    std::string acoustic_file;

    while (std::getline(acoustic_list, acoustic_file)) {
        std::vector<std::vector<double>> acoustics = speech::load_frames(acoustic_file);

        std::vector<speech::segment> segments = speech::load_segments(segment_list);

        for (auto& s: segments) {
            if (int(s.start_time / 1e5) == int(s.end_time / 1e5)) {
                continue;
            }

            int start_time = std::max<int>(0, std::min<int>(acoustics.size(), s.start_time / 1e5));
            int end_time = std::max<int>(0, std::min<int>(acoustics.size(), s.end_time / 1e5));

            auto feature = speech::clarkson_moreno_feature(acoustics, start_time, end_time, 39);

            for (auto &v: feature) {
                output_file << v << " ";
            }
            output_file << std::endl;

            label_file << phone_id.at(s.label) << std::endl;
        }
    }

    output_file.close();
    label_file.close();

    return 0;
}
