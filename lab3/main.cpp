#include "automaton.cpp"

int main() {
    std::vector<int> start_states = {1, 0, 0, 0};
    std::vector<std::vector<std::string>> transition_matrix = {{"0", "a", "0", "0"},
                                                               {"0", "0", "b", "0"},
                                                               {"0", "0", "0", "c"},
                                                               {"0", "0","0", "0"}};
    std::vector<int> end_states = {0, 0, 0, 1};
    automaton test(start_states, transition_matrix, end_states);
    test.show_automaton();
    test.show_like_arrows();

}
