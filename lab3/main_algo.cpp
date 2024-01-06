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

    std::map<std::set<char>, std::pair<std::pair<automaton, bool>, pumps>> alphabet_automatons_prefix;
    std::map<std::set<char>, std::pair<std::pair<automaton, bool>, pumps>> alphabet_automatons_postfix;
public:
    main_algo(const automaton &a, int C, std::set<char> alphabet);
    AutomatonOracle get_oracle();
    std::map<std::set<char>, std::pair<std::pair<automaton, bool>, pumps>> get_prefix_automatons();
    std::map<std::set<char>, std::pair<std::pair<automaton, bool>, pumps>> get_postfix_automatons();
    void show_prefix_automatons();
    void show_postfix_automatons();
    void start_algo(int count_experiment);
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
        nl_algo = NL(orac, combinations[i]);
        auto auto_postfix = nl_algo.getAutomaton(2);
        if (combinations[i].size() == 1){
            alphabet_automatons_prefix.insert(std::make_pair(combinations[i], std::make_pair(std::make_pair(auto_prefix, true), pumps(auto_prefix))));
            alphabet_automatons_postfix.insert(std::make_pair(combinations[i], std::make_pair(std::make_pair(auto_postfix, true), pumps(auto_postfix))));
        } else if (combinations[i].size() > 1){
            int max_prefix = INT_MIN;
            int max_postfix = INT_MIN;
            auto it = combinations[i].begin();
            for (int j = 0; j < combinations[i].size(); j++){
                if (alphabet_automatons_prefix.at(std::set<char>{*it}).first.first.get_transition_matrix().size() > max_prefix){
                    max_prefix = alphabet_automatons_prefix.at(std::set<char>{*it}).first.first.get_transition_matrix().size();
                }
                if (alphabet_automatons_prefix.at(std::set<char>{*it}).first.first.get_transition_matrix().size() > max_postfix){
                    max_postfix = alphabet_automatons_prefix.at(std::set<char>{*it}).first.first.get_transition_matrix().size();
                }
                it++;
            }
            if (auto_prefix.get_transition_matrix().size() > max_prefix + C){
                alphabet_automatons_prefix.insert(std::make_pair(combinations[i], std::make_pair(std::make_pair(auto_prefix, false), pumps({{},{},{}}))));
            } else {
                alphabet_automatons_prefix.insert(std::make_pair(combinations[i], std::make_pair(std::make_pair(auto_prefix, true), pumps(auto_prefix))));
            }
            if (auto_postfix.get_transition_matrix().size() > max_postfix + C){
                alphabet_automatons_postfix.insert(std::make_pair(combinations[i], std::make_pair(std::make_pair(auto_postfix, false), pumps({{},{},{}}))));
            } else {
                alphabet_automatons_postfix.insert(std::make_pair(combinations[i], std::make_pair(std::make_pair(auto_postfix, true), pumps(auto_postfix))));
            }
        }
    }
}

AutomatonOracle main_algo::get_oracle() {
    return orac;
}

std::map<std::set<char>, std::pair<std::pair<automaton, bool>, pumps>> main_algo::get_prefix_automatons() {
    return alphabet_automatons_prefix;
}

std::map<std::set<char>, std::pair<std::pair<automaton, bool>, pumps>> main_algo::get_postfix_automatons() {
    return alphabet_automatons_postfix;
}

void main_algo::show_prefix_automatons() {
    std::cout << std::endl << "Prefix automatons: " << std::endl;
    for(auto & it : alphabet_automatons_prefix){
        std::cout << "{";
        auto it_set = it.first.begin();
        for (int i = 0; i < it.first.size(); i++){
            if (i != it.first.size() - 1){
                std::cout << *it_set << ", ";
            } else {
                std::cout << *it_set << "}:" << std::endl;
            }
            it_set++;
        }
        it.second.first.first.show_like_arrows();
        std::cout << std::endl;
    }
}

void main_algo::show_postfix_automatons() {
    std::cout << std::endl << "Postfix automatons: " << std::endl;
    for(auto & it : alphabet_automatons_postfix){
        std::cout << "{";
        auto it_set = it.first.begin();
        for (int i = 0; i < it.first.size(); i++){
            if (i != it.first.size() - 1){
                std::cout << *it_set << ", ";
            } else {
                std::cout << *it_set << "}:" << std::endl;
            }
            it_set++;
        }
        it.second.first.first.show_like_arrows();
        std::cout << std::endl;
    }
}

void main_algo::start_algo(int count_experiment = 100) {
    // w1u1w2w3u2w4 (w1, u1, w2 from prefix; w3, u2, w4 from postfix)
    std::cout << std::endl << "Start main algo: " << std::endl;
    for (auto &prefix_item : alphabet_automatons_prefix){
        for (auto &postfix_item : alphabet_automatons_postfix){
            std::map<int, pump> prefix_pumps = prefix_item.second.second.get_automaton_pumps();
            std::map<int, pump> postfix_pumps = postfix_item.second.second.get_automaton_pumps();
            if (prefix_item.second.first.second && postfix_item.second.first.second){
                for (auto &prefix_pump : prefix_pumps){
                    for (auto &postfix_pump : postfix_pumps){
                        for (auto &w1 : prefix_pump.second.get_ways_to_cycle()){
                            for (auto &u1 : prefix_pump.second.get_vertex_cycles()){
                                for (auto &w2 : prefix_pump.second.get_ways_from_cycle()){
                                    for (auto &w3 : postfix_pump.second.get_ways_to_cycle()){
                                        for (auto &u2 : postfix_pump.second.get_vertex_cycles()){
                                            for (auto &w4 : postfix_pump.second.get_ways_from_cycle()){
                                                bool flag = false;
                                                for (int k1 = 1; k1 < count_experiment; k1++){
                                                    for (int k2 = 1; k2 < count_experiment; k2++){
                                                        std::string pump_u1;
                                                        for (int i = 0; i < k1; i++){
                                                            pump_u1 += u1;
                                                        }
                                                        std::string pump_u2;
                                                        for (int i = 0; i < k2; i++){
                                                            pump_u2 += u2;
                                                        }
                                                        if (orac.checkMembership(w1 + pump_u1 + w2 + w3 + pump_u2 + w4)){
                                                            flag = true;
                                                        }
                                                    }
                                                }
                                                for (int k2 = 1; k2 < count_experiment; k2++){
                                                    for (int k1 = 1; k1 < count_experiment; k1++){
                                                        std::string pump_u1;
                                                        for (int i = 0; i < k1; i++){
                                                            pump_u1 += u1;
                                                        }
                                                        std::string pump_u2;
                                                        for (int i = 0; i < k2; i++){
                                                            pump_u2 += u2;
                                                        }
                                                        if (orac.checkMembership(w1 + pump_u1 + w2 + w3 + pump_u2 + w4) && flag){
                                                            std::cout << w1 << std::endl << u1 << std::endl << w2 << std::endl << w3 << std::endl << u2 << std::endl << w4 << std::endl;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
