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
            test_automaton(res, regex, out_regex, 25, fout);
        }
        return 0;
    }
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

    TreeNode* tree = build_tree(postfix);

    auto res4 = process_automaton_tree(tree);

    std::cout << std::endl;
    std::cout << std::endl;
    std::string test = automaton_2_regex(res4);
    std::cout << input_regex << std::endl;
    std::cout << test << std::endl;
    test_automaton(res4, input_regex, test, 15, std::cout);
}
