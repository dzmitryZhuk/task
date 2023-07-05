#include <fstream>

#include "common.h"

int config_get_port(const std::string& config_path){
    int res = 0;
    std::ifstream config(config_path);
    if(config.is_open()){
        std::string host;
        config >> host;
        config >> res;
    }
    else{
        throw " failed opening config file";
    }
    return res;
}

std::string config_get_host(const std::string& config_path){
    std::string res;
    std::ifstream config(config_path);
    if(config.is_open()){
        config >> res;
    }
    else{
        throw " failed opening config file";
    }
    return res;
}