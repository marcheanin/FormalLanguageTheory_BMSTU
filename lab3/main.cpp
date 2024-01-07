#include "main_algo.cpp"

int main() {

    int P = 300;
    int C = 10;
    int P_test = 100;

    std::string input_regex;
    std::ifstream fin("input.txt");
    assert(fin.is_open());
    fin >> input_regex;
    if (!input_regex.empty()) {
        std::vector<std::pair<std::string, std::string> > lexemes = lexer(input_regex);

        for (const auto &elem: lexemes) {
            std::cout << elem.first << " ";
        }
        std::cout << std::endl;

        std::vector<std::pair<std::string, std::string> > postfix = to_postfix(lexemes);

        TreeNode *tree = build_tree(postfix);

        auto res = process_automaton_tree(tree);
        auto res_new = old_automaton_to_new(res);

        AutomatonOracle orac;
        orac.setAutomaton(res_new, P);

        main_algo test_algo = main_algo(orac, C);
        test_algo.start_algo(P_test);

        return 0;
    }

    auto pal_oracle = Palindrome_oracle(P, {'a', 'b'});
    auto psp_oracle = Psp_oracle(C);

    main_algo test_algo = main_algo(pal_oracle, C);
    test_algo.start_algo(P_test);

}
