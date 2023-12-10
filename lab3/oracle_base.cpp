//
// Created by march on 06.12.2023.
//

#include <set>

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

    void check_word_in(std::vector < std::vector<std::string> > m, std::vector <int> finals, int v, std::string word) {
        //std::cout << v << " " << word << std::endl;
        if (finals[v] && word.empty()) {
            check_eq = true;
            return;
        }
        if (word.empty()) return;
        char first = word[0];
        word.erase(0, 1);
        for (int i = 0; i < m[v].size(); i++){
            if (m[v][i] != "0" && std::string(1, first) == m[v][i]) {
                check_word_in(m, finals, i, word);
            }
        }
    }

    void set_alphabet() {
        std::set <char> alph;
        auto m = oracle_automaton.get_transition_matrix();
        for (int i = 0; i < m.size(); i++){
            for (int j = 0; j < m[0].size(); j++){
                if (m[i][j] != "0") alph.insert(m[i][j][0]);
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
        for (auto word : words_oracle) {
            check_eq = false;
            check_word_in(m, finals, 0, word);
            if (!check_eq) {
                return word;
            }
        }
        return "None";
    }

    bool checkMembership(const std::string &word) override {
        auto m = oracle_automaton.get_transition_matrix();
        auto finals = oracle_automaton.get_end_states();
        check_eq = false;
        check_word_in(m, finals, 0, word); // v = 0 - стартовое. Если оно будет не 0 или их будет несколько - исправить.
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

    void buildPrefixAutomaton(); // TODO: Саня Швец  -> prefix_automaton
    void buildPostfixAutomaton(); // TODO: Саня швец -> postfix_automaton
};

void AutomatonOracle::buildPrefixAutomaton() {

}

void AutomatonOracle::buildPostfixAutomaton() {

}

void AutomatonOracle::generate_words_rec(const std::string& s, int n) {
    if (s.size() == n ) {
        std::cout << s << std::endl;
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
