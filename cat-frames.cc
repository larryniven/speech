#include <fstream>
#include "ebt/ebt.h"
#include "speech/speech.h"

int main(int argc, char *argv[])
{
    ebt::ArgumentSpec spec {
        "cat-frames",
        "Concatenate frames",
        {
            {"frame-list", "", true},
            {"window-size", "", true}
        }
    };

    if (argc == 1) {
        ebt::usage(spec);
        exit(1);
    }

    std::unordered_map<std::string, std::string> args = ebt::parse_args(argc, argv, spec);

    std::ifstream frame_list { args.at("frame-list") };
    int window_size = std::stoi(args.at("window-size"));

    std::string line;

    while (std::getline(frame_list, line)) {
        std::vector<std::vector<double>> frames = speech::load_frames(line);

        for (int i = 0; i < frames.size(); ++i) {
            std::vector<double> cat_frames;

            for (int j = i - window_size; j <= i + window_size; ++j) {
                int index = std::min<int>(std::max<int>(j, 0), frames.size() - 1);
                auto& v = frames.at(index);
                cat_frames.insert(cat_frames.end(), v.begin(), v.end());
            }

            std::cout << ebt::join(cat_frames, " ") << std::endl;
        }
    }

    return 0;
}
