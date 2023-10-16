//
// Created by march on 04.10.2023.
//

#ifndef PARSER_CPP
#define PARSER_CPP

#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <queue>
#include <stack>

const std::string CONCAT_OP = "·";
const std::string INTERSECT_OP = "∩";


void replace_dots(std::vector <std::pair <std::string, std::string > >& lexemes) {
    std::set <std::string> terms;
    for (const auto& elem : lexemes){
        if (elem.second == "TERM") {
            terms.insert(elem.first);
        }
    }
    std::vector <std::pair <std::string, std::string > > r = {{"(", "BRACKET"}};
    for (const auto& elem : terms) {
        if (r.size() > 1) r.emplace_back("|", "BINARY");
        r.emplace_back(elem, "TERM");
    }
    r.emplace_back(")", "BRACKET");
    bool f;
    while(true) {
        f = false;
        for (int i = 0; i < lexemes.size(); i++) {
            if (lexemes[i].second == "DOT") {
                lexemes.erase(lexemes.begin() + i);
                lexemes.insert(lexemes.begin() + i, r.begin(), r.end());
                f = true;
                break;
            }
        }
        if (!f) break;
    }
}

void replace_lookahead(std::vector <std::pair <std::string, std::string > >& lexemes, int pos){
    int balance = 1;
    bool end_line_flag = false;
    int pos2 = -1;
    int bracket_pos = pos - 1;
    lexemes.insert(lexemes.begin() + bracket_pos, {"(", "BRACKET"});
    pos++;
    lexemes.erase(lexemes.begin()+pos);
    for (int i = pos; i < lexemes.size(); i++) {
        if (lexemes[i].first == "(") balance++;
        if (lexemes[i].first == ")") balance--;
        if (balance == 0){
            if (lexemes[i - 1].second == "END-LINE"){
                end_line_flag = true;
                lexemes.erase(lexemes.begin()+i-1);
                pos2 = i;
            }
            else pos2 = i + 1;
            break;
        }
    }
    assert(pos2 != -1);
    lexemes.erase(lexemes.begin() + pos2);
    if (!end_line_flag){
        lexemes.insert(lexemes.begin()+pos2, {CONCAT_OP, "CONCAT"});
        pos2++;
        lexemes.insert(lexemes.begin()+pos2, {".", "DOT"});
        pos2++;
        lexemes.insert(lexemes.begin()+pos2, {"*", "UNARY"});
        pos2++;
    }
    lexemes.insert(lexemes.begin()+pos2, {INTERSECT_OP, "INTERSECT"});

    pos2++;

    int f = 0;
    for (int i = pos2; i < lexemes.size(); i++){
        if (lexemes[i].first == "*" || lexemes[i].first == CONCAT_OP || lexemes[i].second == "TERM") continue;
        if (lexemes[i].first == "("){
            int b = 1;
            i++;
            while(b != 0){
                if (lexemes[i].first == "(") b++;
                if (lexemes[i].first == ")") b--;
                i++;
            }
        }
        else{
            lexemes.insert(lexemes.begin() + i, {")", "BRACKET"});
            f = 1;
            break;
        }
    }

    if (f == 0) lexemes.emplace_back(")", "BRACKET");
}

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
            if (i == regex.size() - 1) continue;
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
        if (res[i].second == "TERM" && res[i + 1].second == "TERM"      // a · b
            || res[i].first == ")" && res[i + 1].first == "("           // ) · (
            || res[i].second == "UNARY" && res[i + 1].first == "("      // * · (
            || res[i].second == "TERM" && res[i + 1].first == "("       // a · (
            || res[i + 1].second == "TERM" && res[i].first == ")"       // ) · a
            || res[i].first == "*" && res[i+1].second == "TERM"){       // * · a
            res.insert(res.begin()+i+1, {CONCAT_OP, "CONCAT"});
        }
    }
    //for (const auto& i : res){
   //     std::cout << i.first << " ";
    //}
    //std::cout << std::endl;

    bool f;
    while(true){
        f = false;
        for (int i = 0; i < res.size(); i++){
            if (res[i].second == "LOOKAHEAD"){
                replace_lookahead(res, i);
                f = true;
                break;
            }
        }
        if (!f) break;
    }
    //for (const auto& i : res){
    //    std::cout << i.first << " ";
   // }
    //std::cout << std::endl;
    replace_dots(res);

    return res;
}

int get_priority(const std::string& op){
    if (op == "|") return 4;
    if (op == CONCAT_OP) return 2;
    if (op == "*") return 1;
    if (op == INTERSECT_OP) return 3;
    return 5;
}

std::vector <std::pair <std::string, std::string> > to_postfix(const std::vector <std::pair <std::string, std::string > >& lexemes){
    std::vector <std::pair <std::string, std::string> > res;
    std::stack <std::pair <std::string, std::string> > st;
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

#endif //PARSER_CPP
