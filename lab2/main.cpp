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

/*
 * TODO: regex_gen
 * TODO: автомат -> регулярка (Детерминизация? Минимизация? Удаление ловушек?)
 * TODO: операции с автоматами
 * TODO: FFL -> автомат
 * TODO: генерация слов через финальный автомат
 * TODO: написание тест функции через regex-библу, regex_gen и генерации слов через автомат
 */


std::random_device rd;
std::mt19937 gen(rd());
std::map <int, std::string> ops = {{0, "|"},
                                   {1, CONCAT_OP},
                                   {2, "?="},
                                   {3, "*"}};
int random_value(int low, int high)
{
    std::uniform_int_distribution<> dist(low, high);
    return dist(gen);
}

std::string regex_gen(int terms_range, int terms_col, int la_num, int star_deep){
    if (terms_col <= 0) terms_col = 1;
    //std::cout << terms_col << std::endl;
    std::string s;
    //int term_or_op = random_value(0, 1);
    if (terms_col == 1 || terms_col == 2) {
        int c = random_value(0, terms_range);
        s += char('a' + c);
        return s;
    }
    int range = 3;
    if (la_num == 0 && star_deep == 0) range-= 2;
    else if (la_num == 0 || star_deep == 0) range -= 1;
    int rand_term = random_value(0, range);
    if (range == 2 && rand_term == 2 && star_deep != 0){
        rand_term = 3;
    }
    std::string op = ops[rand_term];
    if (op == "*"){
        return "(" + regex_gen(terms_range, terms_col, la_num, star_deep - 1) + ")*";
    }
    if (op == CONCAT_OP){
        return regex_gen(terms_range, terms_col / 2 + 1, la_num, star_deep) + regex_gen(terms_range, terms_col / 2 - 1, la_num, star_deep);
    }
    if (op == "|") {
        return regex_gen(terms_range, terms_col / 2 + 1, la_num, star_deep) + "|" + regex_gen(terms_range, terms_col / 2 - 1, la_num, star_deep);
    }
    std::string symb = "";
    if (random_value(0, 1)) symb = "$";
    return "(?=" + regex_gen(terms_range, terms_col / 3 + 1, la_num - 1, star_deep) + symb + ")" +
                    regex_gen(terms_range, terms_col / 3 * 2 - 1, la_num - 1, star_deep);
}


int main() {
    std::string input_regex;
    std::ifstream fin("input.txt");
    assert(fin.is_open());
    fin >> input_regex;
    if (input_regex.empty()) {
        std::cout << "gen regex:" << std::endl;
        for (int i = 0; i < 10; i++) {
            std::cout << regex_gen(3, 12, 1, 2) << std::endl;
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
}
