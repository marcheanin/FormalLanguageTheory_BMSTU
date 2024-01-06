//
// Created by march on 06.01.2024.
//

class Palindrome_oracle : public Oracle {
private:
    std::vector <char> alphabet = {'a', 'b'};

    bool check_eq = false;
    bool check_eq2 = false;
    std::vector <std::string> words_oracle;

    int P_const;

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

    void check_word_in2(std::vector < std::vector<std::vector<std::string>>> m, std::vector <int> finals, int v, std::string word) {
        //std::cout << v << " " << word << std::endl;
        if (finals[v] && word.empty()) {
            check_eq2 = true;
            return;
        }
        if (word.empty()) return;
        char first = word[0];
        word.erase(0, 1);
        for (int i = 0; i < m[v].size(); i++){
            if (!m[v][i].empty()) {
                for (const auto &e : m[v][i]){
                    if (std::string(1, first) == e){
                        check_word_in2(m, finals, i, word);
                    }
                }
            }
        }
    }

    void generate_words_rec(const std::string& s, int n) {
        if (s.size() == n ) {
            // std::cout << s << std::endl;
            //if (checkMembership(s )) words_oracle.push_back(s);
            words_oracle.push_back(s);
            P_const--;
        }
        if (s.size() == n) return;
        for (auto letter : alphabet) {
            generate_words_rec(s + letter, n);
        }
    }

    void generate_words() {
        int k = 1;
        while (P_const > 0) {
            generate_words_rec("", k);
            k++;
        }
    }


public:

    Palindrome_oracle(int P) {
        P_const = P;
        generate_words();
    }
    void set_alphabet(std::vector <char> alph) {
        alphabet = alph;
    }

    std::vector <char> getAlphabet() override {
        return alphabet;
    }

    bool checkMembership(const std::string& word) override {
        check_eq = false;
        for (int i = 0, j = word.size() - 1; i < j; i++, j--){
            if (word[i] != word[j]) return false;
        }
        check_eq = true;
        return true;
    }
    bool checkPrefixMembership(const std::string& word) override {
        check_eq = true;
        return true;
    }
    bool checkPostfixMembership(const std::string& word) override{
        check_eq = true;
        return true;
    }

    std::string checkEqual(automaton input_automaton, int mode, std::set <char> alph) override {
        auto m = input_automaton.get_transition_matrix();
        auto finals = input_automaton.get_end_states();
        auto start_st = input_automaton.get_start_states();
        for (auto word : words_oracle) {
            bool f = true;
            for (auto letter : word){
                if (!alph.contains(letter)) {
                    f = false;
                    break;
                }
            }
            if (!f) continue;
            check_eq2 = false;
            check_eq = false;
            for (int i = 0; i < start_st.size(); i++)
                if (start_st[i] == 1) {
                    check_word_in2(m, finals, i, word);
                    if (check_eq) continue;
                    if (mode == 0) checkMembership(word);
                    if (mode == 1) checkPrefixMembership(word);
                    if (mode == 2) checkPostfixMembership(word);
                }
            if (check_eq != check_eq2) {
                return word;
            }
        }
        return "None";
    }
};


