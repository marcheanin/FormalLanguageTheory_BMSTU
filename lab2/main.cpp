#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>

std::vector <std::pair <std::string, std::string> > lexer(const std::string& regex) {
    std::vector <std::pair <std::string, std::string> > res;
    std::string s;
    for (int i = 0; i < regex.size(); i++) {
        s = regex[i];
        if (s == "^") continue;
        else if (s == ")" or s == "("){
            res.emplace_back(s, "BRACKET");
        }
        else if (s == "$"){
            res.emplace_back(s, "END-LINE");
        }
        else if (s == "|"){
            res.emplace_back(s, "BINARY");
        }
        else if (regex[i] == '?' && i < regex.size() - 1 && regex[i+1] == '='){
            res.emplace_back("?=", "LOOKAHEAD");
            i++;
        }
        else if (s == "*"){
            res.emplace_back("*", "UNARY");
        }
        else{
            res.emplace_back(s, "TERM");
        }
    }
    for (int i = 0; i < res.size() - 1; i++){
        if (res[i].second == "TERM" && res[i + 1].second == "TERM"){
            res.insert(res.begin()+i+1, {"•", "CONCAT"});
            i++;
        }
    }
    return res;
}

int main() {
    std::string input_regex;
    std::ifstream fin("input.txt");
    assert(fin.is_open());
    fin >> input_regex;
    std::vector <std::pair <std::string, std::string > > lexed = lexer(input_regex);
    for (auto i : lexed){
        std::cout << i.first << " " << i.second << std::endl;
    }
}
