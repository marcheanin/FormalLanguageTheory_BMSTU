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
    std::string problem_suffix;

    Oracle* orac;
    std::set <char> alphabet;

    bool isConsistent();
    bool isComplete();
    void fillTables();
    void extendTables();
    void moveToTop(int pos);
    bool checkAbsorb(std::vector <int> a, std::vector <int> b); //a >= b
    bool check_coverable(int pos);
public:
    explicit NL(AutomatonOracle _orac, std::set <char> _alphabet) {
        orac = new AutomatonOracle(std::move(_orac));
        alphabet = std::move(_alphabet);
    }

    automaton getAutomaton();
};

automaton NL::getAutomaton() {
    E.emplace_back("");
    S.emplace_back("");

    for (auto elem : alphabet) {
        Sa.emplace_back(1, elem);
    }
    extendTables();
    fillTables();

    while(1) {
        bool f_complete = false, f_consist = false;
        while (!f_complete || !f_consist) {
            if (!isComplete()) {
                moveToTop(problem_row_pos);     //переносим непокрытую строку из нижней таблицы в верхнюю
                std::string prefix = Sa[problem_row_pos];
                problem_row_pos = -1;
                for (auto elem : alphabet) {        // дополняем расширенную таблицу приписанными к новой строке буквами из алфавита
                    Sa.emplace_back(prefix + std::string(1, elem));
                    SaxE.emplace_back(E.size());
                }
                fillTables();               // дозаполняем таблицы
                continue;
            } else {
                f_complete = true;
            }
            if (!isConsistent()) {
                E.emplace_back(problem_suffix);             // добавляем суффикс, на котором произошло противоречие
                extendTables()                             // увеличиваем размер таблиц
                fillTables();     // дозаполняем таблицы
            } else {
                f_consist = true;
            }
        }
    }
}

bool NL::isConsistent() {
    for (int i1 = 0; i1 < S.size(); i1++){
        for (int i2 = 0; i2 < S.size(); i2++){
            std::string a, b;
            std::vector <int> row_a2, row_b2;
            if (checkAbsorb(SxE[i1], SxE[i2])) {
                a = S[i1];
                //row_a = SxE[i1];
                b = S[i2];
                //row_b = SxE[i2];
            }
            else{
                a = S[i2];
                //row_a = SxE[i2];
                b = S[i1];
                //row_b = SxE[i1];
            }
            for (int j1 = 0; j1 < S.size() + Sa.size(); j1++){
                std::string a2;
                if (j1 < S.size()){
                    a2 = S[j1];
                    row_a2 = SxE[j1];
                } else {
                    a2 = Sa[j1];
                    row_a2 = SaxE[j1];
                }
                auto suffix = a2.back();
                a2.pop_back();
                if (a != a2) continue;
                a2.push_back(suffix);
                for (int j2 = 0; j2 < S.size() + Sa.size(); j2++){
                    std::string b2;
                    if (j2 < S.size()){
                        b2 = S[j2];
                        row_b2 = SxE[j2];
                    } else {
                        b2 = Sa[j2];
                        row_b2 = SaxE[j2];
                    }
                    auto suffix2 = b2.back();
                    b2.pop_back();
                    if (b2 != b && suffix != suffix2) continue;
                    b2.push_back(suffix2);
                    if (!checkAbsorb(row_a2, row_b2)) {
                        problem_suffix = suffix;
                        return false;
                    }
                }
            }
        }
    }
    return true;
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
            if (SxE[i][j] == -1) {
                if (orac->checkMembership(S[i] + E[j])) SxE[i][j] = 1;
                else SxE[i][j] = 0;
            }
        }
    }

    for (int i = 0; i < SaxE.size(); i++){
        for (int j = 0; j < SaxE[0].size(); j++){
            if (SaxE[i][j] == -1) {
                if (orac->checkMembership(Sa[i] + E[j])) SaxE[i][j] = 1;
                else SaxE[i][j] = 0;
            }
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

bool NL::checkAbsorb(std::vector<int> a, std::vector<int> b) {
    for (int i = 0; i < a.size(); i++) {
        if (a[i] < b[i]) return false;
    }
    return true;
}

void NL::extendTables() {
    for (auto & row : SxE){
        row.resize(E.size(), -1);
    }
    for (auto & row : SaxE){
        row.resize(E.size(), -1);
    }
}
