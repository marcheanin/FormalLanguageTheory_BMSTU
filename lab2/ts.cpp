#include <iostream>
#include <utility>
#include <vector>
#include <iterator>

std::vector<int> get_one_indexes(std::vector<int> vec){
    std::vector<int> result;
    for(int i = 0; i < vec.size(); i++){
        if (vec[i] == 1){
            result.push_back(i);
        }
    }
    return result;
}

class automaton{
private:

    std::vector <int> start_states;
    std::vector <std::vector <std::pair<std::string, bool>>> transition_matrix;
    std::vector <int> end_states;

    std::vector <bool> visited;
public:
    automaton() = default;
    std::vector<int> get_start_states();
    std::vector <std::vector <std::pair<std::string, bool>>> get_transition_matrix();
    std::vector<int> get_end_states();
    std::vector<bool> get_visited();
    void set_transition_matrix(std::vector <std::vector <std::pair<std::string, bool>>> new_matrix);
    void set_start_states(std::vector <int> new_start_states);
    void set_end_states(std::vector <int> new_end_states);
    void print_start_vector();
    void print_transition_matrix();
    void print_end_vector();
    void show_automaton();
    void show_like_arrows();
    void dfs(int vertex);
    void dfs_from_starts();
    void dfs_transpon(int vertex);
    void delete_traps();

    automaton(std::vector<int> p_start_states,
              std::vector<std::vector<std::pair<std::string, bool>>> p_transition_matrix,
              std::vector<int> p_end_states);
};

void automaton::show_automaton() {
    this->print_start_vector();
    std::cout << std::endl;
    this->print_transition_matrix();
    std::cout << std::endl;
    this->print_end_vector();
    std::cout << std::endl;
}

void automaton::show_like_arrows() {
    std::cout << "digraph G {" << std::endl;
    std::cout << "rankdir=\"LR\";" << std::endl;
    for (int i = 0; i < this->transition_matrix.size(); i++){
        for (int j = 0; j < this->transition_matrix.size(); j++){
            if (this->transition_matrix[i][j].first != "0"){
                std::cout << std::to_string(i) << " -> " << std::to_string(j) << " [label=\"" << this->transition_matrix[i][j].first << "\"];" << std::endl;
            }
        }
    }
    for (int i = 0; i < this->end_states.size(); i++){
        if (this->end_states[i]){
            std::cout << std::to_string(i) <<  " [label=\"" << std::to_string(i) << "\", shape=doublecircle];" << std::endl;
        } else {
            std::cout << std::to_string(i) <<  " [label=\"" << std::to_string(i) << "\", shape=circle];" << std::endl;
        }
    }
    std::cout << "}" << std::endl;
}

automaton::automaton(std::vector <int> p_start_states, std::vector <std::vector <std::pair<std::string, bool>>> p_transition_matrix, std::vector <int> p_end_states) {
    start_states = std::move(p_start_states);
    transition_matrix = std::move(p_transition_matrix);
    end_states = std::move(p_end_states);
    visited = std::vector<bool>(start_states.size(), false);
}

std::vector<int> automaton::get_start_states() {
    return start_states;
}

std::vector <std::vector <std::pair<std::string, bool>>> automaton::get_transition_matrix(){
    return transition_matrix;
}

std::vector<int> automaton::get_end_states(){
    return end_states;
}

void automaton::print_start_vector() {
    for (int start_state : start_states){
        std::cout << start_state << " ";
    }
    std::cout << std::endl;
}

void automaton::print_transition_matrix() {
    for (auto & i : transition_matrix){
        for (const auto & j : i){
            std::cout << j.first << " ";
        }
        std::cout << std::endl;
    }
}

void automaton::print_end_vector() {
    for (int end_state : end_states){
        std::cout << end_state << std::endl;
    }
}

void automaton::dfs(int vertex){
    this->visited[vertex] = true;
    for (int j = 0; j < this->transition_matrix[vertex].size(); j++){
        if (this->transition_matrix[vertex][j].first != "0" && !this->visited[j]){
            dfs(j);
        }
    }
}

void automaton::dfs_from_starts() {
    for (int i = 0; i < this->start_states.size(); i++){
        if (this->start_states[i]){
            dfs(i);
        }
    }
}

std::vector<bool> automaton::get_visited() {
    return this->visited;
}

void automaton::dfs_transpon(int vertex) {
    this->visited[vertex] = true;
    for (int i = 0; i < this->transition_matrix[vertex].size(); i++){
        if (this->transition_matrix[i][vertex].first != "0" && !this->visited[i]){
            dfs_transpon(i);
        }
    }
}

void automaton::delete_traps() {
    // ����� ��������� �� ����������������� ������� �� �������� ���������. ���, ������� �� �������� - �������
    for (int i = 0; i < this->visited.size(); i++){
        visited[i] = false;
    }
    for (int i = 0; i < this->end_states.size(); i++){
        if (this->end_states[i]){
            dfs_transpon(i);
        }
    }
    std::vector <bool> need_delete(this->transition_matrix.size(), false);
    for (int i = 0; i < this->visited.size(); i++){
        if (!this->visited[i]){
            need_delete[i] = true;
        }
    }

    int cnt = 0;
    for (int i = 0; i < need_delete.size(); i++){
        if (need_delete[i]){
            this->start_states.erase(this->start_states.begin() + i - cnt);
            this->end_states.erase(this->end_states.begin() + i - cnt);
            this->transition_matrix.erase(this->transition_matrix.begin() + i - cnt);
            cnt += 1;
        }
    }
    cnt = 0;
    for (auto & i : this->transition_matrix){
        for (int j = 0; j < need_delete.size(); j++){
            if (need_delete[j]){
                i.erase(i.begin() + j - cnt);
                cnt += 1;
            }
        }
        cnt = 0;
    }
    for(auto && i : this->visited){
        i = false;
    }
}

void automaton::set_transition_matrix(std::vector<std::vector<std::pair<std::string, bool>>> new_matrix) {
    this->transition_matrix = std::move(new_matrix);
}

void automaton::set_start_states(std::vector<int> new_start_states) {
    this->start_states = new_start_states;
}

void automaton::set_end_states(std::vector<int> new_end_states) {
    this->end_states = new_end_states;
}

automaton intersect_automatons(automaton& auto1, automaton& auto2){
    std::vector<int> start_states(auto1.get_start_states().size() * auto2.get_start_states().size(), 0);
    for (int i = 0; i < auto1.get_start_states().size(); i++){
        for (int j = 0; j < auto2.get_start_states().size(); j++){
            if (auto1.get_start_states()[i] && auto2.get_start_states()[j]){
                start_states[i * auto2.get_end_states().size() + j] = 1;
            }
        }
    }

    std::vector<int> end_states(auto1.get_end_states().size() * auto2.get_end_states().size(), 0);
    for (int i = 0; i < auto1.get_end_states().size(); i++){
        for (int j = 0; j < auto2.get_end_states().size(); j++){
            if (auto1.get_end_states()[i] && auto2.get_end_states()[j]){
                end_states[i * auto2.get_end_states().size() + j] = 1;
            }
        }
    }

    std::vector <std::vector <std::pair<std::string, bool>>> transition_matrix(auto1.get_transition_matrix().size() * auto2.get_transition_matrix().size(), std::vector <std::pair<std::string, bool>> (auto1.get_transition_matrix().size() * auto2.get_transition_matrix().size(), {"0",
                                                                                                                                                                                                                                                                                      false})) ;
    for (int i = 0; i < auto1.get_transition_matrix().size(); i++){
        for (int j = 0; j < auto1.get_transition_matrix()[i].size(); j++){
            for (int k = 0; k < auto2.get_transition_matrix().size(); k++){
                for (int z = 0; z < auto2.get_transition_matrix()[k].size(); z++){
                    if (auto1.get_transition_matrix()[i][j] == auto2.get_transition_matrix()[k][z] && auto1.get_transition_matrix()[i][j].first != "0"){
                        transition_matrix[i * auto2.get_transition_matrix().size() + k][j * auto2.get_transition_matrix().size() + z].first = auto1.get_transition_matrix()[i][j].first;
                    }
                }
            }
        }
    }

    automaton res(start_states, transition_matrix, end_states);
    res.dfs_from_starts();

    std::vector <bool> need_delete(transition_matrix.size(), false);
    for (int i = 0; i < transition_matrix.size(); i++){
        bool flag = true;
        for (int j = 0; j < transition_matrix.size(); j++){
            if (transition_matrix[i][j].first != "0" || transition_matrix[j][i].first != "0"){
                flag = false;
            }
        }
        need_delete[i] = flag;
    }
    for (int i = 0; i < res.get_visited().size(); i++){
        if (!res.get_visited()[i]){
            need_delete[i] = true;
        }
    }

    int cnt = 0;
    for (int i = 0; i < need_delete.size(); i++){
        if (need_delete[i]){
            start_states.erase(start_states.begin() + i - cnt);
            end_states.erase(end_states.begin() + i - cnt);
            transition_matrix.erase(transition_matrix.begin() + i - cnt);
            cnt += 1;
        }
    }
    cnt = 0;
    for (auto & i : transition_matrix){
        for (int j = 0; j < need_delete.size(); j++){
            if (need_delete[j]){
                i.erase(i.begin() + j - cnt);
                cnt += 1;
            }
        }
        cnt = 0;
    }
    for(int i = 0; i < res.get_visited().size(); i++){
        res.get_visited()[i] = false;
    }
    res = {start_states, transition_matrix, end_states};
    res.delete_traps();

    return res;
}

automaton alternative_automatons(automaton& auto1, automaton& auto2){
    if (auto1.get_transition_matrix().empty()){
        return auto2;
    } else if (auto2.get_transition_matrix().empty()){
        return auto1;
    }
    std::vector<int> start_states(auto1.get_start_states().size() + auto2.get_start_states().size() - 1, 0);
    for (int i = 0; i < auto1.get_start_states().size(); i++){
        if (auto1.get_start_states()[i]){
            start_states[i] = 1;
        }
    }
    for (int i = 1; i < auto2.get_start_states().size(); i++){
        if (auto2.get_start_states()[i]){
            start_states[auto1.get_start_states().size() + i - 1] = 1;
        }
    }

    std::vector <std::vector <std::pair<std::string, bool>>> transition_matrix (auto1.get_transition_matrix().size() + auto2.get_transition_matrix().size() - 1, std::vector<std::pair<std::string, bool>>(auto1.get_transition_matrix().size() + auto2.get_transition_matrix().size() - 1, {"0", false}));
    for (int i = 0; i < auto1.get_transition_matrix().size(); i++){
        for (int j = 0; j < auto1.get_transition_matrix().size(); j++){
            transition_matrix[i][j].first = auto1.get_transition_matrix()[i][j].first;
        }
    }
    for (int i = 0; i < auto2.get_transition_matrix().size(); i++){
        for (int j = 1; j < auto2.get_transition_matrix().size(); j++){
            if (i == 0){
                transition_matrix[i][auto1.get_transition_matrix().size() + j - 1].first = auto2.get_transition_matrix()[i][j].first;
            } else {
                transition_matrix[auto1.get_transition_matrix().size() + i - 1][auto1.get_transition_matrix().size() + j - 1].first = auto2.get_transition_matrix()[i][j].first;
            }
        }
    }

    std::vector<int> end_states (auto1.get_end_states().size() + auto2.get_end_states().size() - 1, 0);
    for (int i = 0; i < auto1.get_end_states().size(); i++){
        end_states[i] = auto1.get_end_states()[i];
    }
    for (int i = 0; i < auto2.get_end_states().size(); i++){
        if (i == 0){
            end_states[i] = end_states[i] || auto2.get_end_states()[i];
        } else {
            end_states[auto1.get_end_states().size() + i - 1] = auto2.get_end_states()[i];
        }
    }

    return {start_states, transition_matrix, end_states};
}

automaton concat_automatons(automaton& auto1, automaton& auto2){
    if (auto2.get_transition_matrix().empty()){
        return auto2;
    } else if (auto1.get_transition_matrix().empty()){
        return auto1;
    }
    std::vector<int> start_states(auto1.get_start_states().size() + auto2.get_start_states().size() - 1, 0);
    for (int i = 0; i < auto1.get_start_states().size(); i++){
        if (auto1.get_start_states()[i]){
            start_states[i] = 1;
        }
    }
    for (int i = 1; i < auto2.get_start_states().size(); i++){
        if (auto2.get_start_states()[i]){
            start_states[auto1.get_start_states().size() + i - 1] = 1;
        }
    }

    std::vector <std::vector <std::pair<std::string, bool>>> transition_matrix (auto1.get_transition_matrix().size() + auto2.get_transition_matrix().size() - 1, std::vector<std::pair<std::string, bool>>(auto1.get_transition_matrix().size() + auto2.get_transition_matrix().size() - 1, {"0", false}));
    for (int i = 0; i < auto1.get_transition_matrix().size(); i++){
        for (int j = 0; j < auto1.get_transition_matrix().size(); j++){
            transition_matrix[i][j].first = auto1.get_transition_matrix()[i][j].first;
        }
    }
    for (int i = 1; i < auto2.get_transition_matrix().size(); i++){
        for (int j = 1; j < auto2.get_transition_matrix().size(); j++){
            transition_matrix[auto1.get_transition_matrix().size() + i - 1][auto1.get_transition_matrix().size() + j - 1].first = auto2.get_transition_matrix()[i][j].first;
        }
    }
    for (int i = 0; i < auto1.get_transition_matrix().size(); i++){
        for (int j = 1; j < auto2.get_transition_matrix().size(); j++){
            if (auto1.get_end_states()[i] != 0) {
                transition_matrix[i][auto1.get_transition_matrix().size() + j - 1].first = auto2.get_transition_matrix()[0][j].first;
            }
        }
    }

    std::vector<int> end_states (auto1.get_end_states().size() + auto2.get_end_states().size() - 1, 0);
    if (auto2.get_end_states()[0] == 0){
        for (int i = 1; i < auto2.get_end_states().size(); i++){
            end_states[auto1.get_end_states().size() + i - 1] = auto2.get_end_states()[i];
        }
    } else {
        for (int i = 0; i < auto1.get_end_states().size(); i++){
            end_states[i] = auto1.get_end_states()[i];
        }
        for (int i = 1; i < auto2.get_end_states().size(); i++){
            end_states[auto1.get_end_states().size() + i - 1] = auto2.get_end_states()[i];
        }
    }
    return {start_states, transition_matrix, end_states};
}

automaton iteration_automaton(automaton& auto1){
    if (auto1.get_transition_matrix().empty()){
        return {std::vector<int>(1, 1), std::vector <std::vector <std::pair<std::string, bool>>>(1, std::vector<std::pair<std::string, bool>>(1, {"0", false})), std::vector<int>(1, 1)};
    }
    std::vector<int> start_states = auto1.get_start_states();

    std::vector<int> end_states = auto1.get_end_states();
    end_states[0] = 1;

    std::vector <std::vector <std::pair<std::string, bool>>> transition_matrix (auto1.get_transition_matrix().size(), std::vector<std::pair<std::string, bool>>(auto1.get_transition_matrix().size(), {"0", false}));
    for (int j = 1; j < auto1.get_transition_matrix().size(); j++){
        transition_matrix[0][j].first = auto1.get_transition_matrix()[0][j].first;
    }
    for (int i = 1; i < auto1.get_transition_matrix().size(); i++){
        for (int j = 1; j < auto1.get_transition_matrix().size(); j++){
            if (end_states[i]){
                if (transition_matrix[0][j].first == "0"){
                    transition_matrix[i][j].first = auto1.get_transition_matrix()[i][j].first;
                } else if (auto1.get_transition_matrix()[i][j].first == "0"){
                    transition_matrix[i][j].first = transition_matrix[0][j].first;
                } else {
                    if (transition_matrix[0][j].first != auto1.get_transition_matrix()[i][j].first){
                        transition_matrix[i][j].first = "(" + transition_matrix[0][j].first + "|" + auto1.get_transition_matrix()[i][j].first + ")";
                    } else {
                        transition_matrix[i][j].first = transition_matrix[0][j].first;
                    }
                }
            } else {
                transition_matrix[i][j].first = auto1.get_transition_matrix()[i][j].first;
            }
        }
    }
    return {start_states, transition_matrix, end_states};
}

std::vector<int> get_from_states(std::vector<std::vector<std::pair<std::string, bool>>> matrix, int i){
    std::vector<int> result;
    for(int k = 0; k < matrix.size(); k++){
        if (matrix[k][i].first != "0" && k != i){
            result.push_back(k);
        }
    }
//    std::cout << std::endl << "From: ";
//    for (int i = 0; i < result.size(); i++){
//        std::cout << result[i];
//    }
//    std::cout << std::endl;
    return result;
}

std::vector<int> get_to_states(std::vector<std::vector<std::pair<std::string, bool>>> matrix, int i){
    std::vector<int> result;

    for (int j = 0; j < matrix.size(); j++){
        if (matrix[i][j].first != "0" && i != j){
            result.push_back(j);
        }
    }
//    std::cout << std::endl << "To: ";
//    for (int i = 0; i < result.size(); i++){
//        std::cout << result[i];
//    }
//    std::cout << std::endl;
    return result;
}

void print_matrix(std::vector<std::vector<std::pair<std::string, bool>>> matrix){
    for(auto & i : matrix){
        for(auto & j: i){
            std::cout << j.first << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

automaton remove_state(automaton& auto1, int index){
    std::vector<int> new_start_states = auto1.get_start_states();
    new_start_states.erase(new_start_states.begin() + index);

    std::vector<std::vector<std::pair<std::string, bool>>> new_transition_matrix = auto1.get_transition_matrix();
    for (auto & i : new_transition_matrix){
        i.erase(i.begin() + index);
    }
    new_transition_matrix.erase(new_transition_matrix.begin() + index);

    std::vector<int> new_end_states = auto1.get_end_states();
    new_end_states.erase(new_end_states.begin() + index);

    return {new_start_states, new_transition_matrix, new_end_states};
}

bool check_start_state(automaton& auto1){
    for(int i = 0; i < auto1.get_transition_matrix().size(); i++){
        if (auto1.get_transition_matrix()[i][0].first != "0"){
            return true;
        }
    }
    return false;
}

int sum_end_states(automaton& auto1){
    int res = 0;
    for (int i = 0; i < auto1.get_end_states().size(); i++){
        res += auto1.get_end_states()[i];
    }
    return res;
}

std::vector<int> get_end_states_indexes(automaton& auto1){
    std::vector<int> res;
    for(int i = 0; i < auto1.get_end_states().size(); i++){
        if(auto1.get_end_states()[i] == 1){
            res.push_back(i);
        }
    }
    return res;
}

std::vector<int> get_start_states_indexes(automaton& auto1){
    std::vector<int> res;
    for(int i = 0; i < auto1.get_end_states().size(); i++){
        if(auto1.get_start_states()[i] == 1){
            res.push_back(i);
        }
    }
    return res;
}

std::string automaton_2_regex(const automaton& auto2){
    auto auto1 = auto2;
    auto1.show_like_arrows();
    if (auto1.get_transition_matrix().empty()){
        return "^$";
    }
    if (check_start_state(auto1)){
        std::vector <std::vector <std::pair<std::string, bool>>> new_transition_matrix = auto1.get_transition_matrix();
        for(auto & i : new_transition_matrix){
            i.emplace_back("0", false);
        }
        new_transition_matrix.emplace_back(auto1.get_transition_matrix().size() + 1, std::pair<std::string, bool>("0", false));
        new_transition_matrix[new_transition_matrix.size() - 1][0] = {"eps", false};
        auto1.set_transition_matrix(new_transition_matrix);
        std::vector <int> new_start_states = auto1.get_start_states();
        new_start_states[0] = 0;
        new_start_states.push_back(1);
        auto1.set_start_states(new_start_states);
    }
    // Add final state
    std::vector <std::vector <std::pair<std::string, bool>>> new_transition_matrix = auto1.get_transition_matrix();
    for(auto & i : new_transition_matrix){
        i.emplace_back("0", false);
    }
    new_transition_matrix.emplace_back(auto1.get_transition_matrix().size() + 1, std::pair<std::string, bool>("0", false));
    std::vector<int> end_states_indexes = get_end_states_indexes(auto1);
    for (auto &i : end_states_indexes){
        new_transition_matrix[i][new_transition_matrix.size() - 1].first = "";
    }
    auto1.set_transition_matrix(new_transition_matrix);

    std::vector <int> new_end_states(new_transition_matrix.size(), 0);
    new_end_states[new_end_states.size() - 1] = 1;
    auto1.set_end_states(new_end_states);

    std::vector <int> new_start_states = auto1.get_start_states();
    new_start_states.push_back(0);
    auto1.set_start_states(new_start_states);
    // remove states
    int i = 0;
    while (auto1.get_transition_matrix().size() != 2){
        if (auto1.get_start_states()[i] or auto1.get_end_states()[i]){
            i++;
            continue;
        } else {
            std::vector<int> from_states = get_from_states(auto1.get_transition_matrix(), i);
            std::vector<int> to_states = get_to_states(auto1.get_transition_matrix(), i);

//            std::cout << std::endl << "FOR: " << i;
//            std::cout << std::endl << "FROM: ";
//            for(int i = 0; i < from_states.size(); i++){
//                std::cout << from_states[i] << " ";
//            }
//            std::cout << std::endl << "TO: ";
//            for(int i = 0; i < to_states.size(); i++){
//                std::cout << to_states[i] << " ";
//            }
//            std::cout << std::endl;

            for (int j = 0; j < from_states.size(); j++){
                for (int k = 0; k < to_states.size(); k++){
                    if (auto1.get_transition_matrix()[from_states[j]][to_states[k]].first == "0") {
                        if (auto1.get_transition_matrix()[i][i].first == "0") {
                            auto new_matrix = auto1.get_transition_matrix();
                            new_matrix[from_states[j]][to_states[k]].first = "(" + auto1.get_transition_matrix()[from_states[j]][i].first + auto1.get_transition_matrix()[i][to_states[k]].first + ")";
                            auto1.set_transition_matrix(new_matrix);
                        } else {
                            auto new_matrix = auto1.get_transition_matrix();
                            new_matrix[from_states[j]][to_states[k]].first = "(" + auto1.get_transition_matrix()[from_states[j]][i].first + "(" + auto1.get_transition_matrix()[i][i].first + ")*" + auto1.get_transition_matrix()[i][to_states[k]].first + ")";
                            auto1.set_transition_matrix(new_matrix);
                        }
                    } else {
                        if (auto1.get_transition_matrix()[i][i].first == "0") {
                            auto new_matrix = auto1.get_transition_matrix();
                            new_matrix[from_states[j]][to_states[k]].first = "(" + auto1.get_transition_matrix()[from_states[j]][to_states[k]].first + "|" + auto1.get_transition_matrix()[from_states[j]][i].first + auto1.get_transition_matrix()[i][to_states[k]].first + ")";
                            auto1.set_transition_matrix(new_matrix);
                        } else {
                            auto new_matrix = auto1.get_transition_matrix();
                            new_matrix[from_states[j]][to_states[k]].first = "(" + auto1.get_transition_matrix()[from_states[j]][to_states[k]].first + "|" + auto1.get_transition_matrix()[from_states[j]][i].first + "(" + auto1.get_transition_matrix()[i][i].first + ")*" + auto1.get_transition_matrix()[i][to_states[k]].first + ")";
                            auto1.set_transition_matrix(new_matrix);
                        }
                    }
                }
            }
            auto1 = remove_state(auto1, i);
            i = 0;
        }
    }
    return auto1.get_transition_matrix()[0][1].first;
}