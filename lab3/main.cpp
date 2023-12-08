#include "automaton.cpp"
#include "oracle_base.cpp"

int main() {
    std::vector<int> start_states = {1, 0, 0, 0, 1, 0};
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

    std::map<int, std::vector<std::string>> test_cycles = test.get_cycles();
//    for(int i = 0; i < test.get_transition_matrix().size(); i++){
//        std::cout << "State " << i << std::endl;
//        for (int j = 0; j < test_cycles[i].size(); j++){
//            std::cout << test_cycles[i][j] << std::endl;
//        }
//        std::cout << std::endl;
//    }

    std::map<int, std::vector<std::string>> a = test.get_all_ways_to_all_vertexes();
    for (int i = 0; i < test.get_transition_matrix().size(); i++){
        std::cout << "To vertex " << i << ": ";
        for(int j = 0; j < a[i].size(); j++){
            std::cout << a[i][j] << " ";
        }
        std::cout << std::endl << std::endl;
    }



//    AutomatonOracle orac;
//    orac.setAutomaton(test);
//    std::cout << orac.checkMembership("aa") << std::endl; // 1
//    std::cout << orac.checkMembership("a") << std::endl; // 0
//    std::cout << orac.checkMembership("aacccbcaca") << std::endl; // 1

}
