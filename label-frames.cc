#include <fstream>
#include <vector>
#include <unordered_map>
#include <iostream>
#include "speech/speech.h"

std::vector<std::string> load_ordered_phoneset(std::string filename)
{
    std::ifstream ifs { filename };
    std::vector<std::string> result;

    std::string line;

    while (std::getline(ifs, line)) {
        result.push_back(line);
    }

    return result;
}

int main(int argc, char *argv[])
{
    std::ifstream frame_list { argv[1] };
    std::ifstream segment_list { argv[2] };

    std::vector<std::string> phoneset = load_ordered_phoneset(argv[3]);

    std::unordered_map<std::string, int> phone_id;
    for (int i = 0; i < phoneset.size(); ++i) {
        phone_id[phoneset[i]] = i;
    }

    std::ofstream feature_stream { argv[4] };
    std::ofstream label_stream { argv[5] };

    int dim = std::stoi(argv[6]);

    std::string frame_file;

    int count = 0;

    while (std::getline(frame_list, frame_file)) {
        std::vector<std::vector<double>> frames = speech::load_frames(frame_file, dim);
        std::vector<speech::segment> segments = speech::load_segment_batch(segment_list);

        int cur_seg = 0;

        for (int t = 0; t < frames.size(); ++t) {
            while (cur_seg < segments.size() && t > segments.at(cur_seg).end_time / 1e5) {
                ++cur_seg;
            }

            std::vector<std::vector<double>> fixed_window;

            for (int i = t - 9; i <= t + 9; ++i) {
                int index = std::min<int>(std::max<int>(i, 0), frames.size() - 1);

                fixed_window.push_back(frames.at(index));
            }

            if (cur_seg < segments.size() && segments.at(cur_seg).start_time / 1e5 <= t
                    && t <= segments.at(cur_seg).end_time / 1e5) {

                int mid_time = int((segments.at(cur_seg).start_time
                    + segments.at(cur_seg).end_time) / 2 / 1e5);

                if (t > mid_time) {
                    if (cur_seg == segments.size() - 1) {
                        label_stream << phone_id[segments.at(cur_seg).label]
                            << " " << 999 << std::endl;
                    } else {
                        label_stream << phone_id[segments.at(cur_seg).label]
                            << " " << phone_id[segments.at(cur_seg + 1).label] << std::endl;
                    }
                } else {
                    if (cur_seg == 0) {
                        label_stream << 999
                            << " " << phone_id[segments.at(cur_seg).label] << std::endl;
                    } else {
                        label_stream << phone_id[segments.at(cur_seg - 1).label]
                            << " " << phone_id[segments.at(cur_seg).label] << std::endl;
                    }
                }
            } else {
                label_stream << 999 << " " << 999 << std::endl;
            }

            for (auto v: fixed_window) {
                for (auto e: v) {
                    feature_stream << e << " ";
                }
            }
            feature_stream << std::endl;

            if (count % 200 == 0) {
                std::cerr << count << "\r";
            }
            ++count;
        }
    }

    std::cerr << std::endl;

    return 0;
}
