#include <fstream>

#include "common.h"

int config_get_port(const std::string& config_path){
    int res = 0;
    std::ifstream config(config_path);
    if(config.is_open()){
        config >> res;   
    }
    else{
        throw 1;
    }
    return res;
}