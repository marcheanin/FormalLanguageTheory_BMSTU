#include "oracle_base.cpp"
#include "NL_star.cpp"

#include "lab2_sources/parser.cpp"
#include "lab2_sources/tree.cpp"
#include "lab2_sources/FFL.cpp"

int main() {
//    std::vector<int> start_states = {1, 0, 0, 0, 1, 0};
//    std::vector<std::vector<std::vector<std::string>>>  transition_matrix = {{{}, {"a"}, {}, {}, {}, {}},
//                                                               {{}, {}, {"a"}, {}, {}, {"a"}},
//                                                               {{}, {}, {"c"}, {"b"}, {}, {}},
//                                                               {{}, {"c"}, {}, {}, {"c"}, {}},
//                                                               {{}, {}, {}, {"a"}, {}, {}},
//                                                               {{}, {}, {}, {}, {}, {}}};
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


//    std::vector<int> start_states = {1, 0, 0, 0, 0};
//    std::vector<std::vector<std::vector<std::string>>> transition_matrix = {{{}, {"a", "b"}, {}, {}, {}},
//                                                                            {{}, {}, {"a", "b"}, {}, {}},
//                                                                            {{}, {}, {}, {"a", "b"}, {}},
//                                                                            {{"b"}, {}, {}, {}, {"a"}},
//                                                                            {{}, {}, {}, {}, {"a", "b"}}};
//    std::vector<int> end_states = {0, 1, 1, 1, 0};

    std::string input_regex;
    std::ifstream fin("input.txt");
    assert(fin.is_open());
    fin >> input_regex;

    std::vector <std::pair <std::string, std::string > > lexemes = lexer(input_regex);

    for (const auto& elem : lexemes) {
        std::cout << elem.first <<  " ";
    }
    std::cout << std::endl;

    std::vector <std::pair <std::string, std::string > > postfix = to_postfix(lexemes);

    TreeNode* tree = build_tree(postfix);

    auto res4 = process_automaton_tree(tree);

    auto res4_new = old_automaton_to_new(res4);
    res4_new.show_like_arrows();

    // automaton test(start_states, transition_matrix, end_states);
    // test.show_like_arrows();
    AutomatonOracle orac;
    orac.setAutomaton(res4_new, 300);

    auto pref_auto = orac.getPrefixAutomaton();
    pref_auto.show_like_arrows();

    auto post_auto = orac.getPostfixAutomaton();
    post_auto.show_like_arrows();

    auto nl_algo = NL(orac, std::set <char> {'a', 'b'});
    auto result = nl_algo.getAutomaton(0);
    result.show_like_arrows();


//
//    test.print_all_ways_to_all_vertexes();
//    std::cout << std::endl;
//    test.print_all_ways_from_all_vertexes();
//    std::cout << std::endl << "========================" << std::endl;
//
//    AutomatonOracle orac;
//    orac.setAutomaton(test, 1000);
//    orac.buildPrefixAutomaton(test);
//    orac.buildPostfixAutomaton(test);
//
//    pump pump_3_prefix = pump(3, orac.get_prefix_automaton());
//    pump pump_3_postfix = pump(3, orac.get_postfix_automaton());
//    orac.get_prefix_automaton().show_like_arrows();
//    pump_3_prefix.print_pump();
//    std::cout << std::endl << "========================" << std::endl;
//    orac.get_postfix_automaton().show_like_arrows();
//    pump_3_postfix.print_pump();
}
