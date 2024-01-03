#include <utility>
#include <cassert>

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
    static bool checkAbsorb(std::vector <int> a, std::vector <int> b); //a >= b
    bool check_coverable(int pos, int table_num);
    void printCurrentState();
    bool check_full_coverable(int pos, int table_num);

    automaton buildAutomaton();
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
    SxE.emplace_back();

    for (auto elem : alphabet) {
        Sa.emplace_back(1, elem);
        SaxE.emplace_back();
    }
    extendTables();  // extendTables() НЕ добавляет новые строки
    fillTables();

    std::cout << "Tables inited" << std::endl;
    printCurrentState();

    automaton last_automaton;

    while(1) {
        bool f_complete = false, f_consist = false;
        while (!f_complete || !f_consist) {
            if (!isComplete()) {
                std::cout << "Not complete" << std::endl;
                std::string prefix = Sa[problem_row_pos];
                moveToTop(problem_row_pos);     //переносим непокрытую строку из нижней таблицы в верхнюю

                problem_row_pos = -1;
                for (auto elem : alphabet) {        // дополняем расширенную таблицу приписанными к новой строке буквами из алфавита
                    Sa.emplace_back(prefix + std::string(1, elem));
                    SaxE.emplace_back();
                }
                extendTables();
                fillTables();               // дозаполняем таблицы
                printCurrentState();
                continue;
            } else {
                f_complete = true;
            }
            if (!isConsistent()) {
                std::cout << "Not consistant" << std::endl;
                E.emplace_back(problem_suffix);             // добавляем суффикс, на котором произошло противоречие
                extendTables();                             // увеличиваем размер таблиц
                fillTables();                               // дозаполняем таблицы
                printCurrentState();
            } else {
                f_consist = true;
            }
        }
        std::cout << "Building automaton" << std::endl;
        last_automaton = buildAutomaton();
        last_automaton.show_like_arrows();
        std::cout << "Checking equal" << std::endl;
        std::string eq_result = orac->checkEqual(last_automaton);
        if (eq_result == "None") break;
        else{
            std::cout << "Have example " << eq_result << std::endl;
            for (int i = 1; i < eq_result.size() + 1; i++) {
                E.push_back(eq_result.substr(0, i));
            }
            extendTables();
            fillTables();
            std::cout << "After adding example" << std::endl;
            printCurrentState();
        }
    }
    std::cout << "Building tables complete" << std::endl;
    return last_automaton;
}

bool NL::isConsistent() {
    for (int i1 = 0; i1 < S.size(); i1++){
        for (int i2 = i1 + 1; i2 < S.size(); i2++){
            std::string a, b;
            std::vector <int> row_a2, row_b2;
            if (checkAbsorb(SxE[i1], SxE[i2])) {
                a = S[i1];
                //row_a = SxE[i1];
                b = S[i2];
                //row_b = SxE[i2];
            }
            else if (checkAbsorb(SxE[i2], SxE[i1])) {
                a = S[i2];
                //row_a = SxE[i2];
                b = S[i1];
                //row_b = SxE[i1];
            } // имеем - a поглощает b
            else {
                continue;
            }
            for (int j1 = 0; j1 < S.size() + Sa.size(); j1++){
                std::string a2;
                if (j1 < S.size()){
                    a2 = S[j1];
                    row_a2 = SxE[j1];
                } else {
                    a2 = Sa[j1 - S.size()];
                    row_a2 = SaxE[j1 - S.size()];
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
                        b2 = Sa[j2 - S.size()];
                        row_b2 = SaxE[j2 - S.size()];
                    }
                    auto suffix2 = b2.back();
                    b2.pop_back();
                    if (b2 != b || suffix != suffix2) continue;
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

bool NL::check_coverable(int pos, int table_num) {
    std::vector <int> row;
    if (table_num == 2)
        row = SaxE[pos];
    if (table_num == 1)
        row = SxE[pos];

    std::set <int> right_rows;
    for (int i = 0; i < S.size(); i++){
        if (table_num == 1 && i == pos) continue; // если строка входная из первой таблицы, то не добавляем ее в правильные
        right_rows.insert(i);
    }


    for (int i = 0; i < E.size(); i++){
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
            if (res[i] > 1) res[i] = 1;
        }
    }
    if (res == row) return true;
    else {
        return false;
    }
}

bool NL::check_full_coverable(int pos, int table_num) {
    std::vector <int> row;
    if (table_num == 2)
        row = SaxE[pos];
    if (table_num == 1)
        row = SxE[pos];

    std::set <int> right_rows;
    for (int i = 0; i < S.size(); i++){
        if (table_num == 1 && i == pos) continue; // если строка входная из первой таблицы, то не добавляем ее в правильные
        right_rows.insert(i);
    }

    for (int i = 0; i < Sa.size(); i++){
        if (table_num == 2 && i == pos) continue; // если строка входная из второй таблицы, то не добавляем ее в правильные
        right_rows.insert(i + static_cast <int> (S.size()));
    }


    for (int i = 0; i < E.size(); i++){                 // удаляем строки, у которых стоит единица в суффиксе, где у искомой 0 (верхняя таблица)
        int val = row[i];
        if (val == 1) continue;
        for (int j = 0; j < S.size(); j++){
            if (SxE[j][i] == 1) right_rows.erase(j % static_cast <int> (S.size()));
        }
    }

    for (int i = 0; i < E.size(); i++){                 // удаляем строки, у которых стоит единица в суффиксе, где у искомой 0 (нижняя таблица)
        int val = row[i];
        if (val == 1) continue;
        for (int j = 0; j < Sa.size(); j++){
            if (SaxE[j][i] == 1) right_rows.erase(j + S.size());
        }
    }
    if (right_rows.empty()) return false;
    std::vector <int> res (row.size());
    for (auto num : right_rows) {
        for (int i = 0; i < row.size(); i++){
            if (num < S.size())
                res[i] += SxE[num][i];
            else
                res[i] += SaxE[num - S.size()][i];
            if (res[i] > 1) res[i] = 1;
        }
    }
    if (res == row) return true;
    else {
        return false;
    }
}



bool NL::isComplete() {
    for (int i = 0; i < SaxE.size(); i++) {
        bool f = check_coverable(i, 2);
        if (!f) {
            for (int j = 0; j < Sa.size(); j++){
                if (!check_full_coverable(j, 2)) {
                    problem_row_pos = j;
                    return false;
                }
            }
            assert(problem_row_pos != -1);
            return false;
        }
    }
    return true;
}

void NL::fillTables() {
    for (int i = 0; i < S.size(); i++){
        for (int j = 0; j < E.size(); j++){
            if (SxE[i][j] == -1) {
                if (orac->checkMembership(S[i] + E[j])) SxE[i][j] = 1;
                else SxE[i][j] = 0;
            }
        }
    }

    for (int i = 0; i < Sa.size(); i++){
        for (int j = 0; j < E.size(); j++){
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

automaton NL::buildAutomaton() { // TODO: Марч -> fix function. Пока тут заглушка
    std::vector <std::pair <std::string , std::vector <int> > > states;
    // заполняем список состояний - строки из SxE, не накрываемые остальными из SxE
    for (int i = 0; i < S.size(); i++){
        if (!check_coverable(i, 1))
            states.emplace_back(S[i], SxE[i]);
    }

    std::vector <int> end_states (states.size());
    // состояние финальное, если по суффиксу eps ("") у него 1
    for (int i = 0; i < states.size(); i++){
        if (states[i].second[0] == 1) end_states[i] = 1;
    }

    std::vector <std::vector <std::vector <std::string> > > matrix (states.size(), std::vector < std::vector <std::string>  > (states.size()));

    for (int i = 0; i < S.size(); i++){             //S[i] = u
        for (int j = 0; j < S.size() + Sa.size(); j++){            //Sa[j] = uy
            int index = j;
            std::string uy;
            std::vector <int> row;
            if (j < S.size()) {
                uy = S[j];
                row = SxE[j];
            }
            else {
                index = j - S.size();
                uy = Sa[index];
                row = SaxE[index];
            }

            auto y = uy.back();
            uy.pop_back();
            if (S[i] != uy) continue;
            for (int k = 0; k < S.size(); k++){         //S[k] = v
                auto row2 = SxE[k];
                if (checkAbsorb(row, row2)) {     // если v поглощает uy, добавляем u -> v по y.
                    matrix[i][k].emplace_back(1, y);
                }
            }
        }
    }

    std::vector <int> start_states (states.size());
    start_states[0] = 1;

    for (int i = 0; i < S.size(); i++){
        if (checkAbsorb(SxE[i], SxE[0])) {
            start_states[i] = 1;
        }
    }

    return {start_states, matrix, end_states};
}

void NL::printCurrentState() {              // отладочная
    std::cout << "   ";
    for (const auto & i : E){
        if (i.empty()) std::cout << "eps" << " ";
        else std::cout << i << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < S.size(); i++){
        if (S[i].empty()) std::cout << "eps" << " ";
        else std::cout << S[i] << " ";
        for (int j = 0; j < E.size(); j++){
            std::cout << SxE[i][j] << " ";
        }
        std::cout << std::endl;
    }

    for (int i = 0; i < E.size() * 2; i++){
        std::cout << "-";
    }
    std::cout << std::endl;

    for (int i = 0; i < Sa.size(); i++){
        if (Sa[i].empty()) std::cout << "eps" << " ";
        else std::cout << Sa[i] << " ";
        for (int j = 0; j < E.size(); j++){
            std::cout << SaxE[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
