#include <utility>

//
// Created by march on 07.12.2023.
//


class NL {
private:
    std::vector <std::string> E;
    std::vector <std::string> S;
    std::vector <std::string> Sa;
    std::vector <std::vector <int> > SxE;
    std::vector <std::vector <int> > SaxE;

    int problem_row_pos = -1;

    Oracle* orac;
    std::set <char> alphabet;

    bool isConsistent();
    bool isComplete();
    void fillTables();
    void moveToTop(int pos);
    bool check_coverable(int pos);
public:
    explicit NL(AutomatonOracle _orac) {
        orac = new AutomatonOracle(std::move(_orac));
    }

    void setAlphabet(std::set <char> _alphabet) {
        alphabet = std::move(_alphabet);
    }

    automaton getAutomaton();
};

automaton NL::getAutomaton() {
    E.emplace_back("eps");
    S.emplace_back("eps");

    for (auto elem : alphabet) {
        Sa.emplace_back(1, elem);
        SaxE.emplace_back(E.size());
    }
    fillTables();
    while(1) {
        bool f_complete = false, f_consist = false;
        while (!f_complete || !f_consist) {
            if (!isComplete()) {
                moveToTop(problem_row_pos);
                std::string prefix = Sa[problem_row_pos];
                problem_row_pos = -1;
                for (auto elem : alphabet) {
                    Sa.emplace_back(prefix + std::string(1, elem));
                    SaxE.emplace_back(E.size());
                }
                fillTables();
                continue;
            } else {
                f_complete = true;
            }
            if (!f_consist) {

            } else {
                f_consist = true;
            }
        }
    }
}

bool NL::isConsistent() {
    return false;
}

bool NL::check_coverable(int pos) {
    std::set <int> right_rows;
    for (int i = 0; i < S.size(); i++){
        right_rows.insert(i);
    }
    std::vector <int> row = SaxE[pos];

    for (int i = 0; i < SxE[0].size(); i++){
        int val = row[i];
        if (val == 1) continue;
        for (int j = 0; j < SxE.size(); j++){
            if (SxE[j][i] == 1) right_rows.erase(j);
        }
    }
    if (right_rows.empty()) return false;
    std::vector <int> res (row.size());
    for (auto num : right_rows) {
        for (int i = 0; i < row.size(); i++){
            res[i] += SxE[num][i];
        }
    }
    if (res == row) return true;
    else {
        std::cout << "Strange result in row " << pos << std::endl;
        return true;
    }
}

bool NL::isComplete() {
    for (int i = 0; i < SaxE.size(); i++) {
        bool f = check_coverable(i);
        if (!f) {
            problem_row_pos = i;
            return false;
        }
    }
    return true;
}

void NL::fillTables() {
    for (int i = 0; i < S.size(); i++){
        for (int j = 0; j < E[0].size(); j++){
            if (orac->checkMembership(S[i] + E[j])) SxE[i][j] = 1;
            else SxE[i][j] = 0;
        }
    }

    for (int i = 0; i < SaxE.size(); i++){
        for (int j = 0; j < SaxE[0].size(); j++){
            if (orac->checkMembership(Sa[i] + E[j])) SaxE[i][j] = 1;
            else SaxE[i][j] = 0;
        }
    }

}

void NL::moveToTop(int pos) {
    std::vector <int> row = SaxE[pos];
    std::string prefix = Sa[pos];
    SaxE.erase(SaxE.begin() + pos);
    Sa.erase(Sa.begin() + pos);
    SxE.push_back(row);
    S.push_back(prefix);
}
