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

    std::string frame_file;

    int count = 0;

    while (std::getline(frame_list, frame_file)) {
        std::vector<std::vector<double>> frames = speech::load_frames(frame_file);
        std::vector<speech::segment> segments = speech::load_segments(segment_list);

        speech::segment prev_seg;
        prev_seg.start_time = -1;
        prev_seg.end_time = -1;

        for (auto& s: segments) {
            if (prev_seg.start_time == -1 && prev_seg.end_time == -1) {
                prev_seg = s;
                continue;
            }

            int prev_start_frame = int(prev_seg.start_time / 1e5);
            int prev_end_frame = int(prev_seg.end_time / 1e5);
            int prev_seg_len = prev_end_frame - prev_start_frame;

            int start_frame = int(s.start_time / 1e5);
            int end_frame = int(s.end_time / 1e5);
            int seg_len = end_frame - start_frame;

            for (int t = int(prev_start_frame + 0.5 * prev_seg_len);
                    t < int(start_frame + 0.5 * seg_len); ++t) {

                std::vector<std::vector<double>> fixed_window;

                for (int i = t - 9; i <= t + 9; ++i) {
                    int index = std::min<int>(std::max<int>(i, 0), frames.size() - 1);

                    fixed_window.push_back(frames.at(index));
                }

                label_stream << phone_id[prev_seg.label]
                    << " " << phone_id[s.label] << std::endl;

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

            prev_seg = s;
        }
    }

    std::cerr << std::endl;

    return 0;
}
