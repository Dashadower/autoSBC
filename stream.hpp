#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#ifndef STREAMHANDLER
#define STREAMHANDLER
class StreamHandler {
  public:
    StreamHandler(){};
    StreamHandler(std::stringstream& stan_code) {
      stan_code_ss << stan_code.rdbuf();
    }
    void read_stan_file(std::string stan_file_path){
      std::ifstream fin(stan_file_path.c_str());
      stan_code_ss << fin.rdbuf();
    }
    void read(char* buf, int count){
      stan_code_ss.read(buf, count);
    }
    // return -1 if EOF, else write line without newline to buf string and return 0
    int readline(std::string& buf){
      if(stan_code_ss.eof()){
        return -1;
      }
      std::getline(stan_code_ss, buf);
      return 0;
    }
  private:
    std::stringstream stan_code_ss;
};

#endif