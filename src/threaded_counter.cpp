//
// Created by knidarkness on 28.05.17.
//

#include "../inc/threaded_counter.h"

std::string threaded_counter::clear_word(std::string word){
    size_t i = 0;
    size_t len = word.length();
    while(i < len){
        if (!isalnum(word[i]) || word[i] == ' '){
            word.erase(i,1);
            len--;
        }else
            i++;
    }
    return word;
}

void threaded_counter::reader(std::deque<std::vector<std::string>> &data_blocks, std::string filename, size_t block_size, bool &read_done){
    std::ifstream infile(filename);
    std::string line;
    std::vector<std::string> lines;
    size_t line_index = 0;

    while(getline(infile, line))
    {
        ++line_index;
        boost::algorithm::to_lower(line);
        lines.push_back(boost::trim_copy(line));
        if(line_index == block_size){
            {
                std::lock_guard<std::mutex> ll(mtx);
                data_blocks.push_back(lines);
            }
            cv_read.notify_one();
            line_index = 0;
            lines.clear();
        }
    }
    if (!lines.empty()){
        {
            std::lock_guard<std::mutex> ll(mtx);
            data_blocks.push_back(lines);
        }
        cv_read.notify_one();
    }
    read_done = true;
    cv_read.notify_all();
}

void threaded_counter::counter(std::deque<std::vector<std::string>> &data_blocks, std::deque<std::map<std::string, long>> &merge_queue, bool &read_done, bool &subcountdone){
    while(1) {
        std::unique_lock<std::mutex> lk(mtx);
        if (!data_blocks.empty()) {
            std::vector<std::string> data {(std::vector<std::string> &&) data_blocks.front()};
            std::map<std::string, long> local_data;
            data_blocks.pop_front();
            lk.unlock();

            for(size_t i = 0; i < data.size(); i++){
                std::vector<std::string> results;
                boost::split(results, data.at(i), [](char c){return c == ' ';});

                for(size_t j = 0; j < results.size(); j++){
                    if (!results.at(j).empty() && results.at(j) != " "){
                        local_data[this->clear_word(results.at(j))]++;
                    }
                }
            }
            std::unique_lock<std::mutex> lk_merge(mtx_merge);
            merge_queue.push_back(local_data);
            lk_merge.unlock();
            cv_merge.notify_one();
        } else {
            if (read_done){
                subcountdone = true;
                cv_merge.notify_all();
                break;
            } else {
                cv_read.wait(lk);
            }
        }
    }
}

void threaded_counter::merger(std::map<std::string, long> &global_map, std::deque<std::map<std::string, long>> &reduce_maps, bool &done){
    while(1) {
        std::unique_lock<std::mutex> lk(mtx_merge);
        if (!reduce_maps.empty()) {
            std::map<std::string, long> data {(std::map<std::string, long> &&) reduce_maps.front()};
            reduce_maps.pop_front();
            lk.unlock();

            std::map<std::string, long>::iterator iter = data.begin();
            std::unique_lock<std::mutex> lk_result(mtx_result_map);
            while(iter != data.end()){
                if(global_map.find(iter->first) != global_map.end()){
                    global_map[iter->first] += iter->second;
                } else {
                    global_map[iter->first] = iter->second;
                }
                iter++;
            }
            lk_result.unlock();
        } else {
            if (done){
                break;
            } else {
                cv_merge.wait(lk);
            }
        }
    }
}

std::map<std::string, long> threaded_counter::get_count_data(std::string filename, size_t thread_num, size_t block_size){

    std::deque<std::vector<std::string>> data_blocks;
    std::deque<std::map<std::string, long>> merge_queue;
    std::map<std::string, long> final_count_data;

    std::thread workers[thread_num];

    bool read_done;
    bool sub_count_done;

    std::thread read_thread(&threaded_counter::reader, this, std::ref(data_blocks), filename, block_size, std::ref(read_done));
    std::thread merge_thread(&threaded_counter::merger, this, std::ref(final_count_data), std::ref(merge_queue), std::ref(sub_count_done));

    for(size_t i = 0; i < thread_num; i++){
        workers[i] = std::thread(&threaded_counter::counter, this, std::ref(data_blocks), std::ref(merge_queue), std::ref(read_done), std::ref(sub_count_done));
    }

    read_thread.join();
    for(size_t i = 0; i < thread_num; i++){
        workers[i].join();
    }
    merge_thread.join();

    return final_count_data;
}