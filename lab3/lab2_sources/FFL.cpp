#include "../automaton.cpp"
#include <set>
#include <cassert>
#include "tree.cpp"
#include "ts.cpp"

automaton process_automaton_tree(TreeNode* node){
    if (node->data.second == "UNARY") {
        auto a = process_automaton_tree(node->left);
        return iteration_automaton(a);
    }
    if (node->data.first == CONCAT_OP){
        auto a = process_automaton_tree(node->left);
        auto b = process_automaton_tree(node->right);
        return concat_automatons(a, b);
    }
    if (node->data.first == "|"){
        auto a = process_automaton_tree(node->left);
        auto b = process_automaton_tree(node->right);
        return alternative_automatons(a, b);
    }
    if (node->data.first == INTERSECT_OP){
        auto a = process_automaton_tree(node->left);
        auto b = process_automaton_tree(node->right);
        return intersect_automatons(a, b);
    }
    return FFL(node->data).ffl_2_glushkov();
}




