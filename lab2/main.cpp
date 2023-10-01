#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <queue>
#include <stack>

const std::string CONCAT_OP = "·";

std::vector <std::pair <std::string, std::string> > lexer(const std::string& regex) {
    std::vector <std::pair <std::string, std::string> > res;
    std::string s;
    int balance = 0;

    for (int i = 0; i < regex.size(); i++) {
        s = regex[i];
        if (s == "^") continue;
        else if (s == ")" or s == "(") {
            res.emplace_back(s, "BRACKET");
            if (s == "(") balance++;
            if (s == ")") balance--;
            if (balance < 0) throw std::invalid_argument("Bad bracket balance");
        } else if (s == "$") {
            res.emplace_back(s, "END-LINE");
        } else if (s == "|") {
            res.emplace_back(s, "BINARY");
        } else if (regex[i] == '?' && i < regex.size() - 1 && regex[i + 1] == '=') {
            res.emplace_back("?=", "LOOKAHEAD");
            i++;
        } else if (s == CONCAT_OP) {
            res.emplace_back(CONCAT_OP, "CONCAT");
        } else if (s == "*") {
            res.emplace_back("*", "UNARY");
        } else {
            res.emplace_back(s, "TERM");
        }
    }
    if (balance != 0) throw std::invalid_argument("Bad bracket balance");

    for (int i = 0; i < res.size() - 1; i++){
        if (res[i].second == "TERM" && res[i + 1].second == "TERM"
            || res[i].first == ")" && res[i + 1].first == "("
            || res[i].second == "UNARY" && res[i + 1].first == "("
            || res[i].second == "TERM" && res[i + 1].first == "("
            || res[i + 1].second == "TERM" && res[i].first == ")"){
            res.insert(res.begin()+i+1, {CONCAT_OP, "CONCAT"});
            i++;
        }
    }
    return res;
}

int get_priority(const std::string& op){
    if (op == "|") return 4;
    if (op == CONCAT_OP) return 3;
    if (op == "*") return 2;
    if (op == "?=") return 1;
    return 5;
}


std::vector <std::pair <std::string, std::string> > to_postfix(const std::vector <std::pair <std::string, std::string > >& lexemes){
    std::vector <std::pair <std::string, std::string> > res;
    std::stack <std::pair <std::string, std::string> > st;
    bool unary_f = false;
    for (const auto& lexeme : lexemes) {
        if (lexeme.first == "(") st.push(lexeme);

        else if (lexeme.first == ")") {
            while(st.top().first != "(") {
                res.push_back(st.top());
                st.pop();
            }
            st.pop();
        }

        else if (lexeme.second == "TERM" || lexeme.second == "END-LINE") {
            res.push_back(lexeme);
        }

        else {
            if (lexeme.second != "UNARY")
                while (!st.empty() && get_priority(lexeme.first) >= get_priority(st.top().first)) {
                    res.push_back(st.top());
                    st.pop();
                }
            st.push(lexeme);
        }
    }

    while (!st.empty()) {
        res.push_back(st.top());
        st.pop();
    }

    return res;
}

int main() {
    std::string input_regex;
    std::ifstream fin("input.txt");
    assert(fin.is_open());
    fin >> input_regex;
    std::vector <std::pair <std::string, std::string > > lexemes = lexer(input_regex);
    for (const auto& i : lexemes){
        std::cout << i.first << " " << i.second << std::endl;
    }

    std::vector <std::pair <std::string, std::string > > postfix = to_postfix(lexemes);
    std::cout << std::endl;
    for (const auto& i : postfix){
        std::cout << i.first << " ";
    }
}
