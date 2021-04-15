#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "stream.hpp"
#include "stackparser.hpp"
typedef enum {real, vector, matrix, ordered, simplex, unit_vector, 
              positive_ordered, row_vector, cov_matrix, corr_matrix, 
              choleskey_factor_cov, choleskey_factor} dataTypes; // does not include int


typedef enum {functions, data, transformed_data, parameters, 
              tramsformed_parameters, model, generated_quantities} blockTypes;


int main(void){
  StreamHandler sh;
  sh.read_stan_file("test_stan_models/m1.stan");
  BlockParser bp;
  std::vector<std::pair<std::string, std::string>> blocks;
  bp.parse_first_block(sh, blocks);
  std::cout << blocks.size() << "\n";
  for(auto it = blocks.begin(); it != blocks.end(); it++){
    std::cout << "------------\n";
    std::cout << "block type: " << it->first << "\n";
    std::cout << it->second << "\n";
  }
  
  
  return 0;
}
