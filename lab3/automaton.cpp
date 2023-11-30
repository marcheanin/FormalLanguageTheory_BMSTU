#include <iostream>
#include <utility>
#include <vector>
#include <iterator>

class automaton {
private:

    std::vector<int> start_states;
    std::vector<std::vector<std::string>> transition_matrix;
    std::vector<int> end_states;

public:
    automaton() = default;

    automaton(std::vector<int> p_start_states,
              std::vector<std::vector<std::string>> p_transition_matrix,
              std::vector<int> p_end_states);

    std::vector<int> get_start_states();
    std::vector <std::vector <std::string>> get_transition_matrix();
    std::vector<int> get_end_states();
    void print_start_vector();
    void print_transition_matrix();
    void print_end_vector();
    void show_automaton();
    void show_like_arrows();
};

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
    std::cout << "}" << std::endl;
}

