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

/*
 * TODO: автомат -> регулярка (Удаление состояний)
 * TODO: генерация слов через финальный автомат
 * TODO: написание тест функции через regex-библу, regex_gen и генерации слов через автомат
 */

std::vector <std::string> words;
std::vector <bool> visited (100, false);

void get_words_rec(std::string word, int count, int v, std::vector <int> final_states,  std::vector <std::vector <std::pair<std::string, bool> > > m){
    if (final_states[v]){
        words.push_back(word);
        if (words.size() >= count) return;
    }
    visited[v] = true;
    std::vector <int> nexts;
    for (int j = 0; j < m[v].size(); j++){
        if (m[v][j].first != "0" && !visited[j]){
            nexts.push_back(j);
        }
    }
    if (nexts.empty()) return;
    int val = random_value(0, nexts.size()-1);

    get_words_rec(word + m[v][nexts[val]].first, count, nexts[val], final_states, m);
}

void get_words (automaton a, int count){
    auto m = a.get_transition_matrix();
    auto final = a.get_end_states();

    while(words.size() < count){
        std::fill(visited.begin(), visited.end(), 0);
        get_words_rec("", count, 0, final, m);
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
            std::cout << regex << std::endl;
            std::vector <std::pair <std::string, std::string > > lexemes = lexer(regex);
            std::cout << "Parse completed" << std::endl;
            std::vector <std::pair <std::string, std::string > > postfix = to_postfix(lexemes);
            std::cout << "To postfix completed" << std::endl;
            TreeNode* tree = build_tree(postfix);
            std::cout << "Build tree completed" << std::endl;
            auto res = process_automaton_tree(tree);
           //res.show_automaton();
            res.show_like_arrows();
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


    TreeNode* tree = build_tree(postfix);

//    FFL res = process_tree(tree);
    //auto res2 = process_tree2(tree);

//    if (res2.first.flag != -1)
//        res2.second = res2.first.ffl_2_glushkov();
//
//    res2.first.show();
//    res2.second.show_automaton();
//    res2.second.show_like_arrows();

//    res.show();

    auto res3 = process_automaton_tree(tree);
    auto res4 = process_tree2(tree);
    res4.first.ffl_2_glushkov().show_like_arrows();
    res3.show_automaton();
    res3.show_like_arrows();
    printTree(tree, nullptr, false);

    get_words(res3, 15);
    std::regex r(input_regex);
    for (const auto & word : words){
        std::cout << word << " " << regex_match(word, r) << std::endl;
    }



    std::cout << std::endl;

}
