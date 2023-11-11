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

void test_automaton(automaton a, std::string input_regex, std::string output_regex, int col_words, std::ostream& fout){
    std::vector <std::string> problem_words;
    int col_true = 0;
    if (a.get_end_states().empty()){
        fout << "Empty regex" << std::endl;
        return;
    }
    get_words(a, col_words);
    std::regex r(input_regex);
    std::regex out_r(output_regex);

    for (const auto & word : words){
        bool res = regex_match(word, r);
        //std::cout << word << " " << res << std::endl;
        if (res) col_true++;
        else{
            problem_words.push_back(word);
        }
    }
    fout << "Results for " << input_regex << ": " << std::endl;
    fout << "Words true: " << col_true << "/" << col_words << std::endl;
    if (problem_words.empty()){
        fout << "OK" << std::endl;
        fout << std::endl;
        fout << "Test output regex " << output_regex << std::endl;
        for (const auto & word : words){
            bool res = regex_match(word, out_r);
            //std::cout << word << " " << res << std::endl;
            if (res) col_true++;
            else{
                problem_words.push_back(word);
            }
        }
        if (problem_words.empty()) {
            fout << "OK" << std::endl;
        }
        else{
            fout << "Problem words for output regex:" << std::endl;
            for (const auto& word : problem_words) {
                fout << "\"" << word << "\"" << std::endl;
            }
        }
    }
    else{
        fout << "Problem words:" << std::endl;
        for (const auto& word : problem_words) {
            fout << "\"" << word << "\"" << std::endl;
        }
    }
    fout << std::endl;
    words.clear();
    fout << "===========================================================" << std::endl;
}

int main() {
    //TODO: не забыть обработать пустую регулярку ^$
    std::string input_regex;
    std::ifstream fin("input.txt");
    assert(fin.is_open());
    fin >> input_regex;
    if (input_regex.empty()) {
        std::ofstream fout("test_results.txt");
        std::cout << "gen regex:" << std::endl;
        std::vector <std::string> regexes;
        for (int i = 0; i < 50; i++) {
            regexes.push_back(regex_gen(3, 10, 2, 2));
            //std::cout << regexes.back() << std::endl;
        }
        for (const auto& regex : regexes){
            std::cout << regex << std::endl;
            std::vector <std::pair <std::string, std::string > > lexemes = lexer(regex);
            //std::cout << "Parse completed" << std::endl;
            std::vector <std::pair <std::string, std::string > > postfix = to_postfix(lexemes);
            //std::cout << "To postfix completed" << std::endl;
            TreeNode* tree = build_tree(postfix);
            //std::cout << "Build tree completed" << std::endl;
            auto res = process_automaton_tree(tree);
            auto out_regex = automaton_2_regex(res);
            test_automaton(res, regex, out_regex, 25, fout);
            //res.show_automaton();
            //res.show_like_arrows();
            //printTree(tree, nullptr, false);
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

//    auto res3 = process_automaton_tree(tree);
//    res3.show_automaton();
//    res3.show_like_arrows();
//    printTree(tree, nullptr, false);
//    test_automaton(res3, input_regex, 10, std::cout);



//    auto res3 = process_automaton_tree(tree);
//    res3.show_automaton();
//    res3.show_like_arrows();
//    printTree(tree, nullptr, false);
//
//    std::cout << std::endl;

    auto res4 = process_automaton_tree(tree);
//    res4.show_automaton();
//    res4.show_like_arrows();
    std::cout << std::endl;
    std::cout << std::endl;
    std::string test = automaton_2_regex(res4);
    std::cout << input_regex << std::endl;
    std::cout << test << std::endl;
    test_automaton(res4, input_regex, test, 15, std::cout);
}
