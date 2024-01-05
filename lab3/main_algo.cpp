#include <utility>

#include "oracle_base.cpp"
#include "NL_star.cpp"

#include "lab2_sources/parser.cpp"
#include "lab2_sources/tree.cpp"
#include "lab2_sources/FFL.cpp"
#include <bits/stdc++.h>


class main_algo{
private:
    AutomatonOracle orac;

    std::map<std::set<char>, std::pair<automaton, bool>> alphabet_automatons_prefix;
    std::map<std::set<char>, std::pair<automaton, bool>> alphabet_automatons_postfix;
public:
    main_algo(const automaton &a, int C, std::set<char> alphabet);
    AutomatonOracle get_oracle();
    std::map<std::set<char>, std::pair<automaton, bool>> get_prefix_automatons();
    std::map<std::set<char>, std::pair<automaton, bool>> get_postfix_automatons();
};

void combinationUtil(std::set<char> arr, std::vector<char> data,
                     int start, int end,
                     int index, int r, std::vector<std::set<char>> &res){
    if (index == r){
        std::set<char> data_set(data.begin(), data.end());
        res.push_back(data_set);
        return;
    }

    for (int i = start; i <= end &&
                        end - i + 1 >= r - index; i++){
        auto it = arr.begin();
        for (int z = 0; z < i; z++){
            it++;
        }
        data[index] = *it;
        combinationUtil(arr, data, i+1,
                        end, index+1, r, res);
    }
}

void getCombination(std::set<char> arr, int n, int r, std::vector<std::set<char>> &res){
    std::vector<char> data(r);
    combinationUtil(arr, data, 0, n-1, 0, r, res);
}

void get_all_sets(std::set<char> alphabet, std::vector<std::set<char>> &res){
    int n = (sizeof(char) * alphabet.size())/sizeof(char);
    for (int i = 1; i <= alphabet.size(); i++){
        getCombination(alphabet, n, i, res);
    }
}

main_algo::main_algo(const automaton &a, int C, std::set<char> alphabet) {
    orac.setAutomaton(a, C);

    std::vector<std::set<char>> combinations;
    get_all_sets(alphabet, combinations);
    for (int i = 0; i < combinations.size(); i++){
        auto nl_algo = NL(orac, combinations[i]);
        auto auto_prefix = nl_algo.getAutomaton(1);
        auto auto_postfix = nl_algo.getAutomaton(2);
        if (combinations[i].size() == 1){
            alphabet_automatons_prefix[combinations[i]] = std::pair<automaton, bool>{auto_prefix, true};
            alphabet_automatons_postfix[combinations[i]] = std::pair<automaton, bool>{auto_postfix, true};
        } else if (combinations[i].size() > 1){
            int max_prefix = INT_MIN;
            int max_postfix = INT_MIN;
            auto it = combinations[i].begin();
            for (int j = 0; j < combinations[i].size(); j++){
                if (alphabet_automatons_prefix[std::set<char>{*it}].first.get_transition_matrix().size() > max_prefix){
                    max_prefix = alphabet_automatons_prefix[std::set<char>{*it}].first.get_transition_matrix().size();
                }
                if (alphabet_automatons_postfix[std::set<char>{*it}].first.get_transition_matrix().size() > max_postfix){
                    max_postfix = alphabet_automatons_postfix[std::set<char>{*it}].first.get_transition_matrix().size();
                }
                it++;
            }
            if (auto_prefix.get_transition_matrix().size() > max_prefix + C){
                alphabet_automatons_prefix[combinations[i]] = std::pair<automaton, bool>{auto_prefix, false};
            } else {
                alphabet_automatons_prefix[combinations[i]] = std::pair<automaton, bool>{auto_prefix, true};
            }
            if (auto_postfix.get_transition_matrix().size() > max_postfix + C){
                alphabet_automatons_postfix[combinations[i]] = std::pair<automaton, bool>{auto_postfix, false};
            } else {
                alphabet_automatons_postfix[combinations[i]] = std::pair<automaton, bool>{auto_postfix, true};
            }
        }
    }
}

AutomatonOracle main_algo::get_oracle() {
    return orac;
}

std::map<std::set<char>, std::pair<automaton, bool>> main_algo::get_prefix_automatons() {
    return alphabet_automatons_prefix;
}

std::map<std::set<char>, std::pair<automaton, bool>> main_algo::get_postfix_automatons() {
    return alphabet_automatons_postfix;
}

