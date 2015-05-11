#ifndef SPEECH_UTIL_SPEECH_H
#define SPEECH_UTIL_SPEECH_H

#include <unordered_set>
#include <vector>

namespace speech {

    std::unordered_set<std::string> load_phone_set(std::string filename);

    std::vector<double> clarkson_moreno_feature(
        std::vector<std::vector<double>> utt, int start, int end, int nfeat=39);

    std::vector<std::vector<double>> load_frames(std::string filename, int nfeat=39);

}

#endif
