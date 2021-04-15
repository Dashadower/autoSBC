#ifndef CONSTANTS
#include <string>
namespace constants{

typedef enum {integer = "int", real = "real", vector = "vector", matrix = "matrix", ordered, simplex, unit_vector, 
              positive_ordered, row_vector, cov_matrix, corr_matrix, 
              choleskey_factor_cov, choleskey_factor} dataTypes;


typedef enum {functions, data, transformed_data, parameters, 
              tramsformed_parameters, model, generated_quantities} blockTypes;

}
#endif