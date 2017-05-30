#include <iostream>
#include <map>
#include "inc/threaded_counter.h"
#include "inc/t_measure.h"


struct config{
    std::string input_file;
    std::string output_alpha;
    std::string output_num;
    size_t thread_num;
    size_t block_size;
    bool read_success;
};

config get_conf_from_file(std::string filename){
    std::ifstream infile(filename);
    std::string line;
    std::vector<std::string> lines;
    config conf;

    while(getline(infile, line))
    {
         lines.push_back(line);
    }

    if(lines.size() != 5){
        std::cout << "error reading settings file" << std::endl;
        conf.read_success = false;
        return conf;
    }

    try{
        conf.input_file = lines.at(0);
        conf.output_alpha = lines.at(1);
        conf.output_num = lines.at(2);
        conf.thread_num = (size_t) stol(lines.at(3));
        conf.block_size = (size_t) stol(lines.at(4));
        conf.read_success = true;
    } catch (...){
        std::cout << "error reading settings file" << std::endl;
        conf.read_success = false;
    }
    return conf;
}

int main(){
    config conf = get_conf_from_file("conf.ini");
    if (!conf.read_success){
        std::cout << "error reading settings file" << std::endl;
        return -1;
    }
    threaded_counter counter1;

    auto start_time = get_current_time_fenced();
    std::map<std::string, long> result = counter1.get_count_data(conf.input_file, conf.thread_num, conf.block_size);
    auto end_process_time = get_current_time_fenced();

    std::vector<std::pair<std::string, int>> pairs;

    for (auto itr = result.begin(); itr != result.end(); ++itr)
        pairs.push_back(*itr);

    sort(pairs.begin(), pairs.end(), [=](std::pair<std::string, int>& a, std::pair<std::string, int>& b)
         {
             return a.second > b.second;
         }
    );

    std::ofstream out_val;
    out_val.open(conf.output_num);


    for(size_t i = 0; i < pairs.size(); i++){
        out_val << pairs.at(i).first << " " << pairs.at(i).second << std::endl;
    }
    out_val.close();

    std::ofstream out_alpha;
    out_alpha.open(conf.output_alpha);

    std::map<std::string, long>::iterator iter = result.begin();

    while(iter != result.end()){
        out_alpha << iter->first << " " << iter->second << std::endl;
        iter++;
    }
    out_alpha.close();
    auto end_time = get_current_time_fenced();


    std::cout << "Processing time: " <<  to_ms(end_process_time - start_time) << " milliseconds" << std::endl;
    std::cout << "Writing results time: " <<  to_ms(end_time - end_process_time) << " milliseconds" << std::endl;
    std::cout << "Total time: " <<  to_ms(end_time - start_time) << " milliseconds" << std::endl;
    std::cout << "Done" << std::endl;
    return 0;
}