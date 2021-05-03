#include <algorithm>
#include <stack>
#include <vector>
#include <set>
#include <string>
#include "stream.hpp"
#include "vari_map.hpp"
#ifndef PARSER
#define PARSER


/*
A class which contains a stack of characters, with a helper function
that returns a string by combining its contents, without emptying the stack.
*/
class StringStack {
  public:
    void push(char character){
      char_stack.push_back(character);
    }
    char top(){
      return char_stack.back();
    }

    void pop(){
      char_stack.pop_back();
    }

    void clear(){
      char_stack.clear();
    }
    std::string get_string(){
      return std::string(char_stack.begin(), char_stack.end());
    }
    
    bool is_empty(){
      return char_stack.empty();
    }

    int length(){
      return char_stack.size();
    }

    char& operator[](int index){
      return char_stack[index];
    }

  private:
    std::vector<char> char_stack;
};

/*
A class which separates programs by blocks. 
The simplest task. Just find the highest level brace and separate by them.
I used a character-by-character scanner with a stack to find top-level braces.
*/
class BlockParser {
  public:

    /*
     * Given a well formed stan program, remove single line comments and 
     * separate the program by blocks.
     * Fills a std::vector with std::pair<std::string, std::string> as its type,
     * with the first string containing the block name, and the second string containing
     * stan code, *including the top-level(block-level) braces*
     */
    void parse_first_block(StreamHandler& sh, std::vector<std::pair<std::string, std::string>>& return_block_vec){
      
      std::set<std::string> blocks = {"functions", "data", "transformed", "parameters", 
              "model", "generated"};

      std::stack<std::string> parse_stack;
      std::vector<std::string> return_vec;
      std::string block_type;
      std::string next_line;
      bool in_block = false;
      bool found_transformed = false;
      bool found_generated = false;
      int line_num = 0;
      int open_bracket_count = 0;
      while(1){
        if(in_block && parse_stack.empty()){
          // block termination condition:
          // We're currently in a block, but the parse stack is empty.
          in_block = false;
          std::string ss;
          std::reverse(return_vec.begin(), return_vec.end());
          for(int i = 0; i < return_vec.size(); i++){
            ss += return_vec[i];
          }
          return_block_vec.push_back(std::make_pair<std::string, std::string>(std::forward<std::string>(block_type), std::forward<std::string>(ss)));
          return_vec.clear();
          block_type.clear();
        }
        int ret = sh.readline(next_line);
        std::string temp_word;
        if(ret == -1) break;
        line_num++;
        int line_length = next_line.length();
        int text_increment = 0;
        while(text_increment < line_length || !temp_word.empty()){
          if(temp_word.empty()){
            if(next_line[text_increment] == ' '){
            // whitespace in itself has no meaning
            if(found_transformed && parse_stack.empty()) found_transformed = true;
            else if(!parse_stack.empty()) parse_stack.push(std::string(" "));
            text_increment++;
            }
            else{
              temp_word += next_line[text_increment++];
            }
          }
          else { // check tokenized word for keywords
            if(temp_word == "//"){
              // comment - ignore entire remaining line
              temp_word.clear();
              break;
            }
            else if(temp_word == "{"){
              open_bracket_count++;
              parse_stack.push(temp_word);
              temp_word.clear();
            }
            else if(temp_word == "}"){
              open_bracket_count--;
              parse_stack.push(temp_word);
              temp_word.clear();
              if(open_bracket_count == 0){
                // if we're at the outermost level, take all out until no opening brackets remain
                while(!parse_stack.empty()){
                  std::string s;
                  return_vec.push_back(parse_stack.top());
                  parse_stack.pop();
                  if(return_vec.back() == "{"){
                    open_bracket_count--;
                    if(open_bracket_count == 0) break;
                  }
                }
                open_bracket_count = 0;
              }
            }
            else if(!in_block && blocks.find(temp_word) != blocks.end()){
              // find block statement
              if(!found_transformed && temp_word == "transformed"){
                found_transformed = true;
              }
              else if (!found_generated && temp_word == "generated"){
                found_generated = true;
              }
              else if(found_transformed && (temp_word == "parameters" || temp_word ==  "data")){
                block_type += "transformed ";
                block_type += temp_word;
                in_block = true;
                found_transformed = false;
              }
              else if (!found_transformed && !found_generated) {
                block_type = temp_word;
                in_block = true;
              }
              temp_word.clear();
              continue;
            }
            // end token parsing
            else if(next_line[text_increment] == ' ' || next_line[text_increment] == '{'){
              // whitespace either acts as a delimeter, or has no meaning
              // we can just push it to the queue regardless
              if (found_generated && temp_word == "quantities"){
                in_block = true;
                found_generated = false;
                in_block = true;
                block_type = "generated quantities";
                temp_word.clear();
                continue;
              }
              parse_stack.push(temp_word);
              temp_word.clear();
            }
            else if(text_increment == line_length){
              // if we're past the last character, push remaining and break
              parse_stack.push(temp_word);
              temp_word.clear();
            }
            else{
              // keep count of bracket count
              if(next_line[text_increment] == '{'){
                open_bracket_count++;
              }
              else if(next_line[text_increment] == '}'){
                open_bracket_count--;
              }
              temp_word += next_line[text_increment++];
            }
          }
        }
        if(!parse_stack.empty()) parse_stack.push("\n");
        
      }
      
    }
};


typedef enum declarationState {idle, type_scalar, type_nonscalar, nonscalar_dim,
                               bound, offset, multiplier, name, array_dim} declarationState;

typedef struct VariableContext
{
  std::string name;
  dataTypes varType;
  bool isScalar;
  std::vector<int> dim;
  double lower;
  double upper;
  double offset;
  double multiplier;
  int array_dim;
} VariableContext;


/*
real <lower=0, upper=1> var [1];
type_scalar bound       name dimension

vector<lower=0, upper=5, offset=0, multiplier=1>[5] var2[4]

Data block parser:
9-state, LR parser.
Shift-Reduce parsing for each statement.
Each state represents the position within a single data variable statement.
The parser does not give any meaning to newline characters: semicolons are used
for determining end of statements.

<var_identifier>(<lower=,upper=,offset=,multiplier>)([<var_multidim>]) <var_name>([<var_arrdim>])

*/
class DeclarationParser {
  public:
    DeclarationParser(std::string& block_code) : parser_state(idle), block_code(block_code) {};
    void parse(){
      std::string staging_str;
      char lookahead_char;
      int char_index = 0;

      bool isRunning = true;

      while(isRunning){
        lookahead_char = block_code[char_index++];
        switch(parser_state){
          // idle state: no varlables declared, currently not evaluating any
          // variable declarations or arguments
          case declarationState::idle:
            if(lookahead_char == ' '){
              continue;
            }
            else if(lookahead_char == ';'){
              continue;
            }
            else{
              staging_str += lookahead_char;
            }
            // end single character handling

            if(staging_str == "vector"){

            }

            break;
          
          // identified scalar type declaration
          case declarationState::type_scalar:
            break;

          // identified non-scalar type declaration
          case declarationState::type_nonscalar:
            break;
          
          // entering non-scalar dimension declaration brackets
          case declarationState::nonscalar_dim:
            break;
          
          // entering variable bound declaration
          case declarationState::bound:
            break;
          
          // entering variable name declaration
          case declarationState::name:
            break;
          
          // entering sequential array dimension declaration
          case declarationState::array_dim:
            break;
        }
      }
    }
  private:
    declarationState parser_state;
    const std::string block_code;
    StringStack staging_stack;
};

#endif
