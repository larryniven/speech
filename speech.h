#ifndef SPEECH_UTIL_SPEECH_H
#define SPEECH_UTIL_SPEECH_H

#include <unordered_set>
#include <vector>

namespace speech {

    std::vector<std::string> load_label_batch(std::istream& is);

    struct segment {
        int start_time; // in 1e-7 s
        int end_time;
        std::string label;
    };

    std::vector<segment> load_segments(std::istream& is);

    std::vector<std::vector<double>> load_frames(std::string filename, int nfeat=-1);

    std::vector<std::vector<double>> load_frame_batch(std::istream& is, int nfeat=-1);

    std::vector<std::string> load_label_set(std::string filename);

    std::vector<double> clarkson_moreno_feature(
        std::vector<std::vector<double>> utt, int start, int end, int nfeat=39);

    std::vector<double> clarkson_moreno_feature(
        std::vector<std::vector<double>> utt, int start, int end,
        int start_dim, int end_dim);

}

#endif
