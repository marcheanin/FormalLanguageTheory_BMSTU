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

std::pair <FFL, automaton> process_tree2(TreeNode* node){
    if (node->data.second == "UNARY") {
        auto a = process_tree2(node->left);
        if (a.first.flag == -1) {
            return {a.first, iteration_automaton(a.second)};
        }
        a.first.unary();
        return a;
    }
    if (node->data.second == "TERM") return {FFL(node->data), automaton()};
    auto a = process_tree2(node->left);
    auto b = process_tree2(node->right);
    if (a.first.flag == -1 || b.first.flag == -1 || node->data.first == INTERSECT_OP){ //делаем операции с автоматами
        if (a.first.flag != -1){
            a.first.flag = -1;
            a.second = a.first.ffl_2_glushkov();
        }
        if (b.first.flag != -1){
            b.first.flag = -1;
            b.second = b.first.ffl_2_glushkov();
        }
        if (node->data.first == CONCAT_OP) {
            auto res = concat_automatons(a.second, b.second);
            return {a.first, res};
        }
        if (node->data.first == INTERSECT_OP) {
            auto res = intersect_automatons(a.second, b.second);
            a.first.flag = -1;
            return {a.first, res};
        }
        if (node->data.first == "|") {
            auto res = alternative_automatons(a.second, b.second);
            return {a.first, res};
        }
        throw std::invalid_argument("Oops, can not find operation: " + node->data.first);
    }
    else{                                                                               //делаем операции с FFL
        if (node->data.first == CONCAT_OP){
            a.first.concatenate(b.first);
            return a;
        }
        if (node->data.first == "|"){
            a.first.alternative(b.first);
            return a;
        }
    }
}


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

    //FFL res = process_tree(tree);
    auto res2 = process_tree2(tree);

    res2.first.show();
    res2.second.show_automaton();

    //res.show();

    printTree(tree, nullptr, false);

    std::cout << std::endl;
    std::cout << std::endl;

    //automaton auto_res = res.ffl_2_glushkov();
    //auto_res.show_like_arrows();
}
