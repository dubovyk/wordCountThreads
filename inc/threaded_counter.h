//
// Created by knidarkness on 28.05.17.
//

#ifndef LAB2_2_THREADED_COUNTER_H
#define LAB2_2_THREADED_COUNTER_H

#include <iostream>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <mutex>
#include <algorithm>
#include <functional>
#include <cctype>
#include <thread>
#include <condition_variable>

class threaded_counter{
public:
    std::map<std::string, long> get_count_data(std::string filename, size_t thread_num, size_t block_size);

private:
    std::string clear_word(std::string word);
    void reader(std::deque<std::vector<std::string>> &data_blocks, std::string filename, size_t block_size, bool &read_done);
    void counter(std::deque<std::vector<std::string>> &data_blocks, std::deque<std::map<std::string, long>> &merge_queue, bool &read_done, bool &subcount_done);
    void merger(std::map<std::string, long> &global_map, std::deque<std::map<std::string, long>> &reduce_maps, bool &done);
    std::mutex mtx;
    std::mutex mtx_merge;
    std::mutex mtx_result_map;
    std::condition_variable cv_read;
    std::condition_variable cv_merge;

};


#endif //LAB2_2_THREADED_COUNTER_H
