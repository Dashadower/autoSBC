#include <algorithm>
#include <set>
#include <string>
#include <queue>
#include <deque>
#ifndef DEPENDENCY_TREE
#define DEPENDENCY_TREE

/*
A dependency tree holds the dependence relationship of all variables/parameters/data.
As an example, in `a = b + c`, we would need to know the values of b and c to calculate a.
If we would like to calculate a variable's value, we would recursively ascend up the tree in a breadth-first manner,
since we would need to evaluate the highest-level nodes first.

a b c d
\/ /\/
 e  f
 \ /
  g
would result in eval(g) = g(e, f) = g(e(a, b, c), f(c, d)) = g(e(eval(a), eval(b), eval(c)), f(eval(c), eval(d)))

This might be useful in SBC for a particular parameter, which means we can "clamp" all unnecessary parameters,
and simulate/fit just the necessary parameters.
*/

class VarNode {
  public:
  VarNode(std::string name) : var_name(name) {};
    void register_parent(VarNode* parent_ptr){
      parents.insert(parent_ptr);
    }
    void get_parents(std::queue<VarNode*>& parent_q){
      for(auto it = parents.begin(); it != parents.end(); it++){
        (*it)->get_parents(parent_q);
        parent_q.push(*it);
      }
    }
    const std::string get_name(){
      return var_name;
    }
  private:
  std::string var_name;
  std::set<VarNode*> parents;
};

#endif