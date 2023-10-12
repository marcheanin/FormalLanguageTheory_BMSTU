#include <iostream>
#include <fstream>
#include <cassert>
#include <utility>
#include <vector>
#include <set>
#include <map>
#include <random>

#include "parser.cpp"
#include "tree.cpp"
#include "FFL.cpp"
#include "test.cpp"

/*
 * TODO: regex_gen
 * TODO: автомат -> регулярка (Детерминизация? Минимизация? Удаление ловушек?)
 * TODO: операции с автоматами
 * TODO: FFL -> автомат
 * TODO: генерация слов через финальный автомат
 * TODO: написание тест функции через regex-библу, regex_gen и генерации слов через автомат
 */


int main() {
    std::string input_regex;
    std::ifstream fin("input.txt");
    assert(fin.is_open());
    fin >> input_regex;
    if (input_regex.empty()) {
        std::cout << "gen regex:" << std::endl;
        std::vector <std::string> regexes;
        for (int i = 0; i < 5; i++) {
            regexes.push_back(regex_gen(3, 12, 0, 2));
            std::cout << regexes.back() << std::endl;
        }
        for (const auto& regex : regexes){
            std::vector <std::pair <std::string, std::string > > lexemes = lexer(regex);
            std::vector <std::pair <std::string, std::string > > postfix = to_postfix(lexemes);
            TreeNode* tree = build_tree(postfix);
            FFL res = process_tree(tree);
            res.show();
            printTree(tree, nullptr, false);
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
    std::cout << std::endl;

    TreeNode* tree = build_tree(postfix);

    FFL res = process_tree(tree);

    res.show();

    printTree(tree, nullptr, false);

    std::cout << std::endl;
    std::cout << std::endl;

    automaton auto_res = res.ffl_2_glushkov();
    auto_res.show_automaton();
}
