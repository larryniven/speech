#include "speech/speech.h"
#include <fstream>
#include "ebt/ebt.h"

namespace speech {

    std::vector<std::string> load_label_seq(std::istream& is)
    {
        std::string line;
        std::getline(is, line);

        std::vector<std::string> parts;

        if (is) {
            parts = ebt::split(line);


            if (ebt::startswith(parts.back(), "(") && ebt::endswith(parts.back(), ")")) {
                parts.pop_back();
            }
        }

        return parts;
    }

    std::vector<int> load_label_seq(std::istream& is,
        std::unordered_map<std::string, int> const& label_id)
    {
        std::string line;
        std::getline(is, line);

        std::vector<std::string> parts;

        if (is) {
            parts = ebt::split(line);

            if (ebt::startswith(parts.back(), "(") && ebt::endswith(parts.back(), ")")) {
                parts.pop_back();
            }
        }

        std::vector<int> result;
        for (auto& s: parts) {
            result.push_back(label_id.at(s));
        }

        return result;
    }

    std::vector<int> load_label_seq_batch(std::istream& is,
        std::unordered_map<std::string, int> const& label_id)
    {
        std::vector<int> result;
        std::string line;
    
        std::getline(is, line);
        while (std::getline(is, line) && line != ".") {
            auto parts = ebt::split(line);

            for (auto& e: parts) {
                result.push_back(label_id.at(e));
            }
        }
    
        return result;
    }

    std::vector<std::string> load_label_seq_batch(std::istream& is)
    {
        std::vector<std::string> result;
        std::string line;
    
        std::getline(is, line);
        while (std::getline(is, line) && line != ".") {
            auto parts = ebt::split(line);

            result.insert(result.end(), parts.begin(), parts.end());
        }
    
        return result;
    }

    std::vector<std::string> load_label_batch(std::istream& is)
    {
        std::vector<std::string> result;
        std::string line;
    
        std::getline(is, line);
        while (std::getline(is, line) && line != ".") {
            result.push_back(line);
        }
    
        return result;
    }

    std::vector<segment> load_segment_batch(std::istream& is)
    {
        std::vector<segment> result;
        std::string line;
    
        std::getline(is, line);
        while (std::getline(is, line) && line != ".") {
            auto parts = ebt::split(line);
    
            result.push_back(segment { 
                .start_time = std::stoi(parts[0]),
                .end_time = std::stoi(parts[1]),
                .label = parts[2]
            });
        }
    
        return result;
    }

    std::vector<std::vector<double>> load_frames(std::string filename, int nfeat)
    {
        std::vector<std::vector<double>> result;
        std::ifstream ifs { filename };
    
        std::string line;
        while (std::getline(ifs, line)) {
            std::vector<double> vec;
    
            std::vector<std::string> parts = ebt::split(line);
            for (auto& p: parts) {
                if (vec.size() == nfeat) {
                    break;
                }
                vec.push_back(std::stod(p));
            }
    
            result.push_back(vec);
        }
    
        return result;
    }
    
    std::vector<std::vector<double>> load_frame_batch(std::istream& is, int nfeat)
    {
        std::vector<std::vector<double>> result;
        std::string line;

        // read and discard file name
        std::getline(is, line);
    
        while (std::getline(is, line) && line != ".") {
            std::vector<double> vec;
    
            std::vector<std::string> parts = ebt::split(line);
            for (auto& p: parts) {
                if (vec.size() == nfeat) {
                    break;
                }
                vec.push_back(std::stod(p));
            }
    
            result.push_back(vec);
        }
    
        return result;
    }
    
    std::vector<std::string> load_label_set(std::string filename)
    {
        std::vector<std::string> result;

        std::ifstream ifs{filename};
    
        std::string line;
        while (std::getline(ifs, line)) {
            result.push_back(line);
        }
    
        return result;
    }

    std::unordered_map<std::string, int> load_label_id(std::string filename)
    {
        std::unordered_map<std::string, int> result;
        std::string line;
        std::ifstream ifs { filename };

        int i = 0;
        while (std::getline(ifs, line)) {
            result[line] = i;
            ++i;
        }
    
        return result;
    }

    std::vector<double>
    clarkson_moreno_feature(
        std::vector<std::vector<double>> utt, int start, int end, int start_dim, int end_dim)
    {
        std::vector<double> result;
    
        end = std::min<int>(utt.size(), end);
    
        int size = std::max<int>(0, end - start);

        int dim = end_dim - start_dim + 1;
    
        if (size <= 0) {
            result.resize(dim * 5 + 21);
            result.push_back(1);
            return result;
        }
    
        std::vector<double> vec;
        vec.resize(dim);
    
        for (int i = start; i < size * 0.3 + start; ++i) {
            for (int j = start_dim; j < end_dim + 1; ++j) {
                vec[j - start_dim] += utt[i][j] / int(size * 0.3 + 1);
            }
        }
    
        for (int i = 0; i < vec.size(); ++i) {
            result.push_back(vec[i]);
        }
        vec.clear();
        vec.resize(dim);
    
        for (int i = size * 0.3 + start; i < size * 0.7 + start; ++i) {
            for (int j = start_dim; j < end_dim + 1; ++j) {
                vec[j - start_dim] += utt[i][j] / int(size * 0.4 + 1);
            }
        }
    
        for (int i = 0; i < vec.size(); ++i) {
            result.push_back(vec[i]);
        }
        vec.clear();
        vec.resize(dim);
    
        for (int i = size * 0.7 + start; i < size + start; ++i) {
            for (int j = start_dim; j < end_dim + 1; ++j) {
                vec[j - start_dim] += utt[i][j] / int(size * 0.3 + 1);
            }
        }
    
        for (int i = 0; i < vec.size(); ++i) {
            result.push_back(vec[i]);
        }
        vec.clear();
        vec.resize(dim);
    
        for (int i = std::max<int>(start - 2, 0); i < std::min<int>(start + 2, utt.size()); ++i) {
            for (int j = start_dim; j < end_dim + 1; ++j) {
                vec[j - start_dim] += utt[i][j] / 4;
            }
        }
    
        for (int i = 0; i < vec.size(); ++i) {
            result.push_back(vec[i]);
        }
        vec.clear();
        vec.resize(dim);
    
        for (int i = std::max<int>(end - 2, 0); i < std::min<int>(end + 2, utt.size()); ++i) {
            for (int j = start_dim; j < end_dim + 1; ++j) {
                vec[j - start_dim] += utt[i][j] / 4;
            }
        }
    
        for (int i = 0; i < vec.size(); ++i) {
            result.push_back(vec[i]);
        }
        vec.clear();
        vec.resize(dim);
    
        for (int i = 0; i <= 20; ++i) {
            result.push_back((i == size) ? 1 : 0);
        }
        result.push_back(1);
    
        return result;
    }

    std::vector<double>
    clarkson_moreno_feature(
        std::vector<std::vector<double>> utt, int start, int end, int nfeat)
    {
        return clarkson_moreno_feature(utt, start, end, 0, nfeat - 1);
    }

    void skip_batch(std::istream& is, int k)
    {
        int i = 0;

        if (k <= 0) {
            return;
        }

        std::string line;
        while (std::getline(is, line)) {
            if (line == ".") {
                ++i;
            }

            if (i == k) {
                break;
            }
        }
    }

    void batch_indices::open(std::string filename)
    {
        std::ifstream ifs { filename };
    
        std::string line;
        std::getline(ifs, line);
    
        if (!ifs) {
            std::cerr << "unable to open " << filename << std::endl;
            exit(1);
        }
    
        stream.open(line);
    
        if (!stream) {
            std::cerr << "unable to open " << line << std::endl;
            exit(1);
        }
    
        while (std::getline(ifs, line)) {
            pos.push_back(std::stoul(line));
        }
    
        ifs.close();
    }

    std::istream& batch_indices::at(int i)
    {
        stream.seekg(pos.at(i));
        return stream;
    }

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
            ifs_ = std::make_shared<std::ifstream>(std::ifstream { entries[i].filename });
        }

        ifs_->seekg(entries[i].shift);

        return *ifs_;
    }

}
