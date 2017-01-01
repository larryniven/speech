#ifndef SPEECH_UTIL_SPEECH_H
#define SPEECH_UTIL_SPEECH_H

#include <unordered_set>
#include <vector>
#include <fstream>

namespace speech {

    std::vector<std::string> load_label_batch(std::istream& is);

    struct segment {
        long start_time;
        long end_time;
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

    void skip_batch(std::istream& is, int k);

    struct batch_indices {
        std::ifstream stream;
        std::vector<unsigned long> pos;
    
        void open(std::string filename);
        std::ifstream& at(int i);
    };

}

#endif
