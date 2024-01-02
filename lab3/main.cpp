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
    std::vector<std::vector<std::vector<std::string>>> transition_matrix = {{{"b"}, {"a"}, {}, {}, {}, {}, {}, {}},
                                                                           {{}, {}, {"a"}, {}, {}, {"b"}, {}, {}},
                                                                           {{}, {}, {}, {"a"}, {"b"}, {}, {}, {}},
                                                                           {{}, {}, {}, {"a"}, {"b"}, {}, {}, {}},
                                                                           {{}, {}, {}, {}, {}, {}, {"a"}, {"b"}},
                                                                           {{}, {}, {}, {}, {}, {}, {"a"}, {"b"}},
                                                                           {{}, {}, {"a"}, {}, {}, {"b"}, {}, {}},
                                                                           {{"b"}, {"a"}, {}, {}, {}, {}, {}, {}}};
    std::vector<int> end_states = {0, 0, 0, 1, 1, 0, 1, 1};

    automaton test(start_states, transition_matrix, end_states);

    test.print_all_ways_to_all_vertexes();
    std::cout << std::endl;
    test.print_all_ways_from_all_vertexes();
    std::cout << std::endl << "========================" << std::endl;

    AutomatonOracle orac;
    orac.setAutomaton(test, 1000);
    orac.buildPrefixAutomaton(test);
    orac.buildPostfixAutomaton(test);

    pump pump_3_prefix = pump(3, orac.get_prefix_automaton());
    pump pump_3_postfix = pump(3, orac.get_postfix_automaton());
    orac.get_prefix_automaton().show_like_arrows();
    pump_3_prefix.print_pump();
    std::cout << std::endl << "========================" << std::endl;
    orac.get_postfix_automaton().show_like_arrows();
    pump_3_postfix.print_pump();
}
