//
// Created by march on 06.12.2023.
//

#include <set>

class Oracle{
public:
    virtual std::string checkEqual(const automaton& input_automaton) = 0;
    virtual bool checkMembership(const std::string& word) = 0;
};

class AutomatonOracle : public Oracle {
private:
    automaton oracle_automaton;
    automaton prefix_automaton;
    automaton postfix_automaton;

    std::set <char> alphabet;

    bool check_eq = false;

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

    void get_alphabet() {
        auto m = oracle_automaton.get_transition_matrix();
        for (int i = 0; i < m.size(); i++){
            for (int j = 0; j < m[0].size(); j++){
                if (m[i][j] != "0") alphabet.insert(m[i][j][0]);
            }
        }
    }

public:
    void setAutomaton(const automaton &input_automaton){
        oracle_automaton = input_automaton;
        get_alphabet();
    }

    std::string checkEqual(const automaton &input_automaton) override {}

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
