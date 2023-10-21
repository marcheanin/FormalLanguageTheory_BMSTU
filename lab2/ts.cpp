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
    // Ѕудем проходить по транспонированной матрице из конечных состо€ний. ¬се, которые не покрасим - удал€ем
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

std::vector<int> get_ways_from_state(std::vector<std::vector<std::pair<std::string, bool>>> matrix, int i, std::vector<int> deleted){
    std::vector<int> result;
    for(int k = 0; k < matrix.size(); k++){
        if (matrix[k][i].first != "0" && std::find(deleted.begin(), deleted.end(), k) == deleted.end() && k != i){
            result.push_back(k);
        }
    }
    return result;
}

std::vector<int> get_ways_to_state(std::vector<std::vector<std::pair<std::string, bool>>> matrix, int i, std::vector<int> deleted){
    std::vector<int> result;
    for (int j = 0; j < matrix.size(); j++){
        if (matrix[i][j].first != "0" && std::find(deleted.begin(), deleted.end(), j) == deleted.end() && i != j){
            result.push_back(j);
        }
    }
    return result;
}

std::vector<std::vector<std::pair<std::string, bool>>> fill_state_with_zeroes(std::vector<std::vector<std::pair<std::string, bool>>> matrix, int i){
    for (int j = 0; j < matrix[i].size(); j++){
        matrix[i][j].first = "0";
        matrix[j][i].first = "0";
    }
    return matrix;
}

void print_matrix(std::vector<std::vector<std::pair<std::string, bool>>> matrix){
    for(auto & i : matrix){
        for(auto & j: i){
            std::cout << j.first << " ";
        }
        std::cout << std::endl;
    }
}

std::string automaton_2_regex(automaton& auto1){
    if (auto1.get_transition_matrix().empty()){
        return "^$";
    }
    std::vector<std::vector<std::pair<std::string, bool>>> ts_matrix = auto1.get_transition_matrix();
    //control deleted states for no recursive
    std::vector<int> deleted_states;

    for (int i = 0; i < ts_matrix.size(); i++){
        if (auto1.get_start_states()[i] == 0 && auto1.get_end_states()[i] == 0){
            std::vector<int> to_states = get_ways_to_state(ts_matrix, i, deleted_states);
            std::string to_transitions;
            if (to_states.empty()){
                continue;
            } else if (to_states.size() == 1){
                to_transitions = ts_matrix[i][to_states[0]].first;
            } else {
                to_transitions = "(";
                for (int to_state : to_states){
                    to_transitions += ts_matrix[i][to_state].first + "|";
                }
                to_transitions.pop_back();
                to_transitions += ")";
            }
            std::vector<int> from_states = get_ways_from_state(ts_matrix, i, deleted_states);
            std::string path_from_to;
            if (ts_matrix[i][i].first != "0") {
                for (int from_state: from_states) {
                    std::string from_transit = ts_matrix[from_state][i].first;
                    for (int to_state: to_states) {
                        path_from_to += "(" + from_transit;
                        path_from_to += "(" + (ts_matrix[i][i].first) + ")*";
                        path_from_to += to_transitions + ")";
                        if (!ts_matrix[from_state][to_state].second) {
                            ts_matrix[from_state][to_state].first = path_from_to;
                            ts_matrix[from_state][to_state].second = true;
                        } else {
                            ts_matrix[from_state][to_state].first =
                                    "(" + ts_matrix[from_state][to_state].first + "|" + path_from_to + ")";
                        }
                        path_from_to = "";
                    }
                }
            } else {
                for (int from_state: from_states) {
                    std::string from_transit = ts_matrix[from_state][i].first;
                    for (int to_state: to_states) {
                        path_from_to += "(" + from_transit;
                        path_from_to += to_transitions + ")";
                        if (!ts_matrix[from_state][to_state].second) {
                            ts_matrix[from_state][to_state].first = path_from_to;
                            ts_matrix[from_state][to_state].second = true;
                        } else {
                            ts_matrix[from_state][to_state].first =
                                    "(" + ts_matrix[from_state][to_state].first + "|" + path_from_to + ")";
                        }
                        path_from_to = "";
                    }
                }
            }
            std::cout << std::endl;
            ts_matrix = fill_state_with_zeroes(ts_matrix, i);
            deleted_states.push_back(i);
        }
    }

    std::vector<int> end_states = auto1.get_end_states();
    int result_index;
    for (int i = 0; i < end_states.size(); i++){
        if (end_states[i] == 1){
            result_index = i;
        }
    }
    return ts_matrix[0][result_index].first;
}