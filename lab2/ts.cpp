#include <iostream>
#include <utility>
#include <vector>

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
    std::vector <std::vector <std::string>> transition_matrix;
    std::vector <int> end_states;
public:
    automaton(std::vector <int> p_start_states, std::vector <std::vector <std::string>> p_transition_matrix, std::vector <int> p_end_states);
    std::vector<int> get_start_states();
    std::vector <std::vector <std::string >> get_transition_matrix();
    std::vector<int> get_end_states();
    void print_start_vector();
    void print_transition_matrix();
    void print_end_vector();
    void show_automaton();
    void show_like_arrows();
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
    std::cout << "rankdir=\"LR\";" << std::endl;
    for (int i = 0; i < this->transition_matrix.size(); i++){
        for (int j = 0; j < this->transition_matrix.size(); j++){
            if (this->transition_matrix[i][j] != "0"){
                std::cout << std::to_string(i) << " -> " << std::to_string(j) << " [label=\"" << this->transition_matrix[i][j] << "\"];" << std::endl;
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
}

automaton::automaton(std::vector <int> p_start_states, std::vector <std::vector <std::string>> p_transition_matrix, std::vector <int> p_end_states) {
    start_states = std::move(p_start_states);
    transition_matrix = std::move(p_transition_matrix);
    end_states = std::move(p_end_states);
}

std::vector<int> automaton::get_start_states() {
    return start_states;
}

std::vector <std::vector <std::string>> automaton::get_transition_matrix(){
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
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }
}

void automaton::print_end_vector() {
    for (int end_state : end_states){
        std::cout << end_state << std::endl;
    }
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
    std::vector<int> end_indexes1 = get_one_indexes(auto1.get_end_states());
    std::vector<int> end_indexes2 = get_one_indexes(auto2.get_end_states());
    std::vector<int> end_states(auto1.get_end_states().size() * auto2.get_end_states().size(), 0);
    for(int i : end_indexes1){
        for (int j : end_indexes2){
            end_states[i * auto2.get_end_states().size() + j] = 1;
        }
    }
    std::vector<std::vector<std::string>> transition_matrix(auto1.get_transition_matrix().size() * auto2.get_transition_matrix().size(), std::vector <std::string> (auto1.get_transition_matrix().size() * auto2.get_transition_matrix().size(), "0")) ;
    for (int i = 0; i < auto1.get_transition_matrix().size(); i++){
        for (int j = 0; j < auto1.get_transition_matrix()[i].size(); j++){
            for (int k = 0; k < auto2.get_transition_matrix().size(); k++){
                for (int z = 0; z < auto2.get_transition_matrix()[k].size(); z++){
                    if (auto1.get_transition_matrix()[i][j] == auto2.get_transition_matrix()[k][z] && auto1.get_transition_matrix()[i][j] != "0"){
                        transition_matrix[i * auto2.get_transition_matrix().size() + k][j * auto2.get_transition_matrix().size() + z] = auto1.get_transition_matrix()[i][j];
                    }
                }
            }
        }
    }

    return {start_states, transition_matrix, end_states};
}

automaton alternative_automatons(automaton& auto1, automaton& auto2){
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

    std::vector <std::vector <std::string>> transition_matrix (auto1.get_transition_matrix().size() + auto2.get_transition_matrix().size() - 1, std::vector<std::string>(auto1.get_transition_matrix().size() + auto2.get_transition_matrix().size() - 1, "0"));
    for (int i = 0; i < auto1.get_transition_matrix().size(); i++){
        for (int j = 0; j < auto1.get_transition_matrix().size(); j++){
            transition_matrix[i][j] = auto1.get_transition_matrix()[i][j];
        }
    }
    for (int i = 0; i < auto2.get_transition_matrix().size(); i++){
        for (int j = 1; j < auto2.get_transition_matrix().size(); j++){
            if (i == 0){
                transition_matrix[i][auto1.get_transition_matrix().size() + j - 1] = auto2.get_transition_matrix()[i][j];
            } else {
                transition_matrix[auto1.get_transition_matrix().size() + i - 1][auto1.get_transition_matrix().size() + j - 1] = auto2.get_transition_matrix()[i][j];
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

    std::vector <std::vector <std::string>> transition_matrix (auto1.get_transition_matrix().size() + auto2.get_transition_matrix().size() - 1, std::vector<std::string>(auto1.get_transition_matrix().size() + auto2.get_transition_matrix().size() - 1, "0"));
    for (int i = 0; i < auto1.get_transition_matrix().size(); i++){
        for (int j = 0; j < auto1.get_transition_matrix().size(); j++){
            transition_matrix[i][j] = auto1.get_transition_matrix()[i][j];
        }
    }
    for (int i = 1; i < auto2.get_transition_matrix().size(); i++){
        for (int j = 1; j < auto2.get_transition_matrix().size(); j++){
            transition_matrix[auto1.get_transition_matrix().size() + i - 1][auto1.get_transition_matrix().size() + j - 1] = auto2.get_transition_matrix()[i][j];
        }
    }
    for (int i = 0; i < auto1.get_transition_matrix().size(); i++){
        for (int j = 1; j < auto2.get_transition_matrix().size(); j++){
            if (auto1.get_end_states()[i] != 0) {
                transition_matrix[i][auto1.get_transition_matrix().size() + j - 1] = auto2.get_transition_matrix()[0][j];
            }
        }
    }
    for (int i = 1; i < auto1.get_transition_matrix().size(); i++){
        for (int j = 1; j < auto2.get_transition_matrix().size(); j++){
            transition_matrix[auto1.get_transition_matrix().size() + i - 1][auto1.get_transition_matrix().size() + j - 1] = auto2.get_transition_matrix()[i][j];
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
    std::vector<int> start_states = auto1.get_start_states();

    std::vector<int> end_states = auto1.get_end_states();
    end_states[0] = 1;

    std::vector <std::vector <std::string>> transition_matrix (auto1.get_transition_matrix().size(), std::vector<std::string>(auto1.get_transition_matrix().size(), "0"));
    for (int j = 1; j < auto1.get_transition_matrix().size(); j++){
        transition_matrix[0][j] = auto1.get_transition_matrix()[0][j];
    }
    for (int i = 1; i < auto1.get_transition_matrix().size(); i++){
        for (int j = 1; j < auto1.get_transition_matrix().size(); j++){
            if (end_states[i]){
                if (transition_matrix[0][j] == "0"){
                    transition_matrix[i][j] = auto1.get_transition_matrix()[i][j];
                } else if (auto1.get_transition_matrix()[i][j] == "0"){
                    transition_matrix[i][j] = transition_matrix[0][j];
                } else {
                    transition_matrix[i][j] = "(" + transition_matrix[0][j] + "|" + auto1.get_transition_matrix()[i][j] + ")";
                }
            } else {
                transition_matrix[i][j] = auto1.get_transition_matrix()[i][j];
            }
        }
    }
    return {start_states, transition_matrix, end_states};
}
