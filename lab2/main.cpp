#include <iostream>
#include <fstream>
#include <cassert>
#include <utility>
#include <vector>
#include <set>
#include <map>
#include <random>
#include <regex>

#include "parser.cpp"
#include "tree.cpp"
#include "FFL.cpp"
#include "test.cpp"

int main() {
    std::string input_regex;
    std::ifstream fin("input.txt");
    assert(fin.is_open());
    fin >> input_regex;
    if (input_regex.empty()) {
        std::ofstream fout("test_results.txt");
        std::cout << "gen regex:" << std::endl;
        std::vector <std::string> regexes;
        for (int i = 0; i < 250; i++) {
            regexes.push_back(regex_gen(3, 15, 3, 3));
        }
        for (const auto& regex : regexes){
            std::cout << regex << std::endl;
            std::vector <std::pair <std::string, std::string > > lexemes = lexer(regex);

            std::vector <std::pair <std::string, std::string > > postfix = to_postfix(lexemes);

            TreeNode* tree = build_tree(postfix);

            auto res = process_automaton_tree(tree);
            auto out_regex = automaton_2_regex(res);
            test_automaton(res, regex, out_regex, 100, fout);
        }
        return 0;
    }
    std::vector <std::pair <std::string, std::string > > lexemes = lexer(input_regex);

    std::vector <std::pair <std::string, std::string > > postfix = to_postfix(lexemes);

    TreeNode* tree = build_tree(postfix);

    auto res4 = process_automaton_tree(tree);

    std::cout << std::endl;
    std::cout << std::endl;
    std::string test = automaton_2_regex(res4);
    test_automaton(res4, input_regex, test, 100, std::cout);
}
