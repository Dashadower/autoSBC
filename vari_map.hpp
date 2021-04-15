#include <map>
#include "constants.hpp"

class VariableData {
  public:
    VariableData(std::string name, constants::dataTypes data_type, bool defined,
                 bool is_parameter) : var_name(name), var_data_type(data_type),
                                      var_defined(defined), 
                                      var_is_parameter(is_parameter) {};

    VariableData(std::string name, constants::dataTypes data_type, bool defined)
                 : var_name(name), var_data_type(data_type),
                 var_defined(defined), var_is_parameter(false) {};

  private:
    std::string var_name;
    constants::dataTypes var_data_type;
    bool var_defined;
    bool var_is_parameter;
};

class VariableMap {
  public:
    VariableMap(std::string map_scope) : map_scope_s(std::move(map_scope)) {}
    VariableMap(const std::string& map_scope) : map_scope_s(map_scope) {}


  private:
    std::string map_scope_s;
    std::map<std::string, VariableData* >
};