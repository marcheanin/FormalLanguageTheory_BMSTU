#include "automaton.cpp"
#include "oracle_base.cpp"
#include "NL_star.cpp"

int main() {
//    std::vector<int> start_states = {1, 0, 0, 0, 1, 0};
//    std::vector<std::vector<std::string>> transition_matrix = {{"0", "a", "0", "0", "0", "0"},
//                                                               {"0", "0", "a", "0", "0", "a"},
//                                                               {"0", "0", "c", "b", "0", "0"},
//                                                               {"0", "c", "0", "0", "c", "0"},
//                                                               {"0", "0", "0", "a", "0", "0"},
//                                                               {"0", "0", "0", "0", "0", "0"}};
//    std::vector<int> end_states = {0, 0, 0, 0, 0, 1};
//
//    automaton test(start_states, transition_matrix, end_states);
//    test.show_automaton();
//    test.show_like_arrows();
    std::vector<int> start_states = {1, 0, 0, 0, 0, 0, 0, 0};
    std::vector<std::vector<std::string>> transition_matrix = {{"b", "a", "0", "0", "0", "0", "0", "0"},
                                                               {"0", "0", "a", "0", "0", "b", "0", "0"},
                                                               {"0", "0", "0", "a", "b", "0", "0", "0"},
                                                               {"0", "0", "0", "a", "b", "0", "0", "0"},
                                                               {"0", "0", "0", "0", "0", "0", "a", "b"},
                                                               {"0", "0", "0", "0", "0", "0", "a", "b"},
                                                               {"0", "0", "a", "0", "0", "b", "0", "0"},
                                                               {"b", "a", "0", "0", "0", "0", "0", "0"}};
    std::vector<int> end_states = {0, 0, 0, 1, 1, 0, 1, 1};

    automaton test(start_states, transition_matrix, end_states);

    test.print_all_ways_to_all_vertexes();
    std::cout << std::endl;
    test.print_all_ways_from_all_vertexes();

    AutomatonOracle orac;
    orac.setAutomaton(test, 1000);
    orac.buildPrefixAutomaton(test);
    orac.buildPostfixAutomaton(test);
}
