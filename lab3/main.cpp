#include "automaton.cpp"

int main() {
    std::vector<int> start_states = {1, 0, 0, 0, 0, 0};
    std::vector<std::vector<std::string>> transition_matrix = {{"0", "a", "0", "0", "0", "0"},
                                                               {"0", "0", "a", "0", "0", "a"},
                                                               {"0", "0", "c", "b", "0", "0"},
                                                               {"0", "c", "0", "0", "c", "0"},
                                                               {"0", "0", "0", "a", "0", "0"},
                                                               {"0", "0", "0", "0", "0", "0"}};
    std::vector<int> end_states = {0, 0, 0, 0, 0, 1};
    automaton test(start_states, transition_matrix, end_states);
    test.show_automaton();
    test.show_like_arrows();

//    std::vector<std::string> a;
//    test.get_cycle("", 4, 4, a);

    std::map<int, std::vector<std::string>> test_cycles = test.get_cycles();
    for(int i = 0; i < test.get_transition_matrix().size(); i++){
        std::cout << "State " << i << std::endl;
        for (int j = 0; j < test_cycles[i].size(); j++){
            std::cout << test_cycles[i][j] << std::endl;
        }
        std::cout << std::endl;
    }
}
