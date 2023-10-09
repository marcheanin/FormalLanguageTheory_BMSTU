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

automaton intersect_automatons(automaton& auto1, automaton& auto2){
    std::vector<int> start_states(auto1.get_start_states().size() * auto2.get_start_states().size(), 0);
    start_states[0] = 1;
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

int main(){
    //testing
    std::vector <int> automaton1_start {1, 0, 0};
    std::vector <std::vector <std::string>> automaton1_matrix {{"0", "a", "b"}, {"0", "0", "b"}, {"0", "a", "0"}};
    std::vector <int> automaton1_end {0, 1, 0};
    automaton automaton1 = automaton(automaton1_start, automaton1_matrix, automaton1_end);

    std::vector <int> automaton2_start {1, 0};
    std::vector <std::vector <std::string>> automaton2_matrix {{"0", "a"}, {"b", "0"}};
    std::vector <int> automaton2_end {0, 1};
    automaton automaton2 = automaton(automaton2_start, automaton2_matrix, automaton2_end);

    automaton res = intersect_automatons(automaton1, automaton2);
    res.print_start_vector();
    std::cout << std::endl;
    res.print_transition_matrix();
    std::cout << std::endl;
    res.print_end_vector();
    return 0;
}