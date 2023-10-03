#include <iostream>
#include <utility>
#include <vector>

class automaton{
private:
    std::vector <int> start_states;
    std::vector <int> end_states;
    std::vector <std::vector <std::string>> transition_matrix;
public:
    automaton(std::vector <int> p_start_states, std::vector <std::vector <std::string>> p_transition_matrix, std::vector <int> p_end_states);
    std::vector<int> get_start_states();
    std::vector<int> get_end_states();
    std::vector <std::vector <std::string >> get_transition_matrix();
};

automaton::automaton(std::vector <int> p_start_states, std::vector <std::vector <std::string>> p_transition_matrix, std::vector <int> p_end_states) {
    start_states = std::move(p_start_states);
    end_states = std::move(p_end_states);
    transition_matrix = std::move(p_transition_matrix);
}

std::vector<int> automaton::get_start_states() {
    return start_states;
}

std::vector<int> automaton::get_end_states(){
    return end_states;
}

std::vector <std::vector <std::string>> automaton::get_transition_matrix(){
    return transition_matrix;
}

//automaton intersect_automatons(automaton& auto1, automaton& auto2){
//    std::vector<int> start_states(auto1.get_start_states().size() * auto2.get_start_states().size(), 0);
//    start_states[0] = 1;
//
//}

int main(){
    std::cout << "abc" << std::endl;
}