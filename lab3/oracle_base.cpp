//
// Created by march on 06.12.2023.
//

#include <set>
#include "pump.cpp"

class Oracle{
public:
    virtual std::string checkEqual(automaton input_automaton) = 0;
    virtual bool checkMembership(const std::string& word) = 0;
    virtual std::vector <char> getAlphabet() = 0;
};

class AutomatonOracle : public Oracle {
private:
    automaton oracle_automaton;
    automaton prefix_automaton;
    automaton postfix_automaton;

    std::vector <char> alphabet;
    std::set <std::string> words_oracle;
    int C_const;

    bool check_eq = false;

    void generate_words();
    void generate_words_rec(const std::string& s, int n);
    void generatePermutes(int k);

    void check_word_in(std::vector < std::vector<std::vector<std::string>>> m, std::vector <int> finals, int v, std::string word) {
        //std::cout << v << " " << word << std::endl;
        if (finals[v] && word.empty()) {
            check_eq = true;
            return;
        }
        if (word.empty()) return;
        char first = word[0];
        word.erase(0, 1);
        for (int i = 0; i < m[v].size(); i++){
            if (!m[v][i].empty()) {
                for (const auto &e : m[v][i]){
                    if (std::string(1, first) == e){
                        check_word_in(m, finals, i, word);
                    }
                }
            }
        }
    }

    void set_alphabet() {
        std::set <char> alph;
        auto m = oracle_automaton.get_transition_matrix();
        for (int i = 0; i < m.size(); i++){
            for (int j = 0; j < m[0].size(); j++){
                if (!m[i][j].empty()){
                    for (const auto &e : m[i][j]){
                        alph.insert(e[0]);
                    }
                }
            }
        }
        for (auto elem : alph) {
            alphabet.push_back(elem);
        }
    }

public:
    void setAutomaton(const automaton &input_automaton, int C){
        oracle_automaton = input_automaton;
        C_const = C;
        set_alphabet();
        generate_words();
    }

    std::vector <char> getAlphabet() override {
        return alphabet;
    }

    std::string checkEqual(automaton input_automaton) override {
        auto m = input_automaton.get_transition_matrix();
        auto finals = input_automaton.get_end_states();
        auto start_st = input_automaton.get_start_states();
        for (auto word : words_oracle) {
            check_eq = false;
            for (int i = 0; i < start_st.size(); i++)
                if (start_st[i] == 1) {
                    check_word_in(m, finals, i, word);
                    if (check_eq) break;
                }
            if (!check_eq) {
                return word;
            }
        }
        return "None";
    }

    bool checkMembership(const std::string &word) override {
        auto m = oracle_automaton.get_transition_matrix();
        auto start_st = oracle_automaton.get_start_states();
        auto finals = oracle_automaton.get_end_states();
        check_eq = false;
        for (int i = 0; i < start_st.size(); i++)
            if (start_st[i] == 1)
                check_word_in(m, finals, i, word); // v = 0 - стартовое. Если оно будет не 0 или их будет несколько - исправить.
        return check_eq;
    }

    bool checkPrefixMembership(const std::string  &word) {
        auto m = prefix_automaton.get_transition_matrix();
        auto finals = prefix_automaton.get_end_states();
        check_eq = false;
        check_word_in(m, finals, 0, word); // v = 0 - стартовое. Если оно будет не 0 или их будет несколько - исправить.
        return check_eq;
    }
    bool checkPostfixMembership(const std::string  &word) {
        auto m = postfix_automaton.get_transition_matrix();
        auto finals = postfix_automaton.get_end_states();
        check_eq = false;
        check_word_in(m, finals, 0, word); // v = 0 - стартовое. Если оно будет не 0 или их будет несколько - исправить.
        return check_eq;
    }

    void buildPrefixAutomaton(automaton &input_automaton); // TODO: Саня Швец  -> prefix_automaton
    void buildPostfixAutomaton(automaton &input_automaton); // TODO: Саня швец -> postfix_automaton
    automaton get_prefix_automaton();
    automaton get_oracle_automaton();
    automaton get_postfix_automaton();
};

void AutomatonOracle::buildPrefixAutomaton(automaton &input_automaton) {
    std::vector<int> start_states = input_automaton.get_start_states();
    std::vector<std::vector<std::vector<std::string>>> transition_matrix = input_automaton.get_transition_matrix();
    std::vector<int> end_states = input_automaton.get_end_states();
    for (int i = 0; i < input_automaton.get_end_states().size(); i++){
        end_states[i] = 1;
    }
    prefix_automaton = automaton(start_states, transition_matrix, end_states);
}

void AutomatonOracle::buildPostfixAutomaton(automaton &input_automaton) {
    automaton result_automaton;
    std::vector<int> start_states = input_automaton.get_start_states();
    std::vector<std::vector<std::vector<std::string>>> transition_matrix = input_automaton.get_transition_matrix();
    std::vector<std::vector<std::vector<std::string>>> transition_matrix_transpose;
    for (int i = 0; i < transition_matrix.size(); i++){
        std::vector<std::vector<std::string>> elem;
        for (int j = 0; j < transition_matrix.size(); j++){
            elem.push_back(transition_matrix[j][i]);
        }
        transition_matrix_transpose.push_back(elem);
    }
    std::vector<int> end_states = input_automaton.get_end_states();

    for (int i = 0; i < end_states.size(); i++){
        if (end_states[i]){
            start_states[i] = 1;
        } else {
            start_states[i] = 0;
        }
    }
    for (int i = 0; i < input_automaton.get_end_states().size(); i++){
        end_states[i] = 1;
    }
    postfix_automaton = automaton(start_states, transition_matrix_transpose, end_states);
}

void AutomatonOracle::generate_words_rec(const std::string& s, int n) {
    if (s.size() == n ) {
       // std::cout << s << std::endl;
        if (checkMembership(s )) words_oracle.insert(s);
        C_const--;
    }
    if (s.size() == n) return;
    for (auto letter : alphabet) {
        generate_words_rec(s + letter, n);
    }
}

void AutomatonOracle::generate_words() {
    int k = 1;
    while (C_const > 0) {
        generate_words_rec("", k);
        k++;
    }
}

automaton AutomatonOracle::get_prefix_automaton(){
    return prefix_automaton;
};

automaton AutomatonOracle::get_oracle_automaton(){
    return oracle_automaton;
};

automaton AutomatonOracle::get_postfix_automaton(){
    return postfix_automaton;
};

