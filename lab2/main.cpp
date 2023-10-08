#include <iostream>
#include <fstream>
#include <cassert>
#include <utility>
#include <vector>
#include <set>
#include <map>

#include "parser.cpp"
#include "tree.cpp"
#include "FFL.cpp"


int main() {
    std::string input_regex;
    std::ifstream fin("input.txt");
    assert(fin.is_open());
    fin >> input_regex;
    std::vector <std::pair <std::string, std::string > > lexemes = lexer(input_regex);

    for (const auto& i : lexemes){
        std::cout << i.first << " ";
    }

    std::vector <std::pair <std::string, std::string > > postfix = to_postfix(lexemes);
    std::cout << std::endl;
    for (const auto& i : postfix){
        std::cout << i.first << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;

    TreeNode* tree = build_tree(postfix);

    FFL res = process_tree(tree);

    res.show();

    printTree(tree, nullptr, false);
}
