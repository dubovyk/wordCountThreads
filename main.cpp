#include <iostream>
#include <map>
#include "inc/threaded_counter.h"

struct config{
    std::string input_file;
    std::string output_alpha;
    std::string output_num;
    size_t thread_num;
    size_t block_size;
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
    }

    conf.input_file = lines.at(0);
    conf.output_alpha = lines.at(1);
    conf.output_num = lines.at(2);
    conf.thread_num = (size_t) stol(lines.at(3));
    conf.block_size = (size_t) stol(lines.at(4));
    return conf;
}

int main(){
    config conf = get_conf_from_file("conf.ini");
    threaded_counter counter1;
    counter1.get_count_data(conf.input_file, conf.thread_num, conf.block_size);
    return 0;
}