#include <map>
#include <limits>
#include <vector>
#include "constants.hpp"

/*
A Variable map holds a map for all data/parameters, whether the values are known or not,
to either a value or an expression. If I'm going to run simulations independently of stan;
that is, actually run simulation code in C++, this means I would also have to keep track of
its values/expressions. I chose this as the intermediate representation form.

My plan is to have a default, scalar/matrix VariableData class, and a templated ArrayData class,
which will use a std::vector to create a vector of VariableData types. And an ArrayDate class of ArrayData classes
for 2d arrays, and so on.

Internally, I'm planning to use Eigen data types to represent non-scalar data, since all of stan's
variables' dimensions are known at compile time.

I'm going to have to take a closer look at at the Stan Math library if I want to
keep compatibility between complex data types like simplex, ordered, and others.
*/

class VariableData {
  public:
    VariableData(std::string name, constants::dataTypes data_type, bool defined,
                 bool is_parameter) : var_name(name), var_data_type(data_type),
                                      var_is_defined(defined), 
                                      var_is_parameter(is_parameter) {};

    VariableData(std::string name, constants::dataTypes data_type, bool defined)
                 : var_name(name), var_data_type(data_type),
                 var_is_defined(defined), var_is_parameter(false) {};

  private:
    std::string var_name;
    constants::dataTypes var_data_type;
    bool var_is_defined;
    bool var_is_parameter;
    double lower_bound;
    double upper_bound;
};

template <class var_type>
class ArrayVariable {

};