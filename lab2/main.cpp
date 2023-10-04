#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>

#include "parser.cpp"

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
}
