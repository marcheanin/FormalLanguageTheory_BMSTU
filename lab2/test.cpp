//
// Created by march on 09.10.2023.
//

#include <boost/regex.hpp>

std::random_device rd;
std::mt19937 gen(rd());

std::map <int, std::string> ops = {{0, "|"},
                                   {1, CONCAT_OP},
                                   {2, "?"},
                                   {3, "*"}};
int random_value(int low, int high){
    std::uniform_int_distribution<> dist(low, high);
    return dist(gen);
}

std::string regex_gen(int terms_range, int terms_col, int la_num, int lb_num, int star_deep) {
    std::string symb;
    if (terms_col <= 0) terms_col = 1;
    std::string s;
    if (terms_col == 1 || terms_col == 2) {
        int c = random_value(0, terms_range);
        s += char('a' + c);
        return s;
    }
    int range = 3;
    if (la_num == 0 && lb_num == 0 && star_deep == 0) range -= 2;
    else if ((la_num == 0 && lb_num == 0) || star_deep == 0) range -= 1;
    int rand_term = random_value(0, range);
    if (range == 2 && rand_term == 2 && star_deep != 0) {
        rand_term = 3;
    }
    std::string op = ops[rand_term];
    if (op == "*") {
        return "(" + regex_gen(terms_range, terms_col, 0, lb_num, star_deep - 1) + ")*";
    }
    if (op == CONCAT_OP) {
        return regex_gen(terms_range, terms_col / 2 + 1, la_num, lb_num, star_deep) +
               regex_gen(terms_range, terms_col / 2 - 1, la_num, lb_num, star_deep);
    }
    if (op == "|") {
        return regex_gen(terms_range, terms_col / 2 + 1, la_num, lb_num, star_deep) + "|" +
               regex_gen(terms_range, terms_col / 2 - 1, la_num, lb_num, star_deep);
    }
    bool la_or_lb = random_value(0, 1);
    if ((!la_or_lb || lb_num == 0) && la_num != 0) {
        if (random_value(0, 1)) symb = "$";
        return "(?=" + regex_gen(terms_range, terms_col / 3 + 1, 0, 0, star_deep) + symb + ")" +
               regex_gen(terms_range, terms_col / 3 * 2 - 1, la_num - 1, lb_num, star_deep);
    }
    else{
        if (random_value(0, 1)) symb = "^";
        return regex_gen(terms_range, terms_col / 3 * 2 - 1, la_num, lb_num - 1, star_deep) +
               "(?<=" + symb + regex_gen(terms_range, terms_col / 3 + 1, 0, 0, star_deep) + ")";
    }
}

std::vector <std::string> words;
std::vector <bool> visited (100, false);

void get_words_rec(std::string word, int count, int v, std::vector <int> final_states,  std::vector <std::vector <std::pair<std::string, bool> > > m){
    if (final_states[v]){
        words.push_back(word);
        if (words.size() >= count) return;
    }
    visited[v] = true;
    std::vector <int> nexts;
    for (int j = 0; j < m[v].size(); j++){
        if (m[v][j].first != "0" && !visited[j]){
            nexts.push_back(j);
        }
    }
    if (nexts.empty()) return;
    int val = random_value(0, nexts.size()-1);

    get_words_rec(word + m[v][nexts[val]].first, count, nexts[val], final_states, m);
}

void get_words (automaton a, int count){
    auto m = a.get_transition_matrix();
    auto final = a.get_end_states();

    while(words.size() < count){
        std::fill(visited.begin(), visited.end(), 0);
        get_words_rec("", count, 0, final, m);
    }
}

bool check_lb(const std::string& regex, int pos){
    int balance = 1;
    for (int i = pos; i < regex.size(); i++){
        if (balance == 0) break;
        if (regex[i] == '(') balance++;
        if (regex[i] == ')') balance--;
        if (regex[i] == '*' || regex[i] == '|') return false;
    }
    return true;
}

bool find_lb_with_ops(const std::string & regex){
    bool f = false;
    for (int i = 0; i < regex.size() - 1; i++){
        if (regex[i] == '?' && regex[i + 1] == '<') {
            if (!check_lb(regex, i)) return true;
        }
    }
    return false;
}

void test_automaton(automaton a, const std::string& input_regex, const std::string& output_regex, int col_words, std::ostream& fout){
    std::vector <std::string> problem_words;
    bool lb_flag = find_lb_with_ops(input_regex);
    boost::smatch match;
    int col_true = 0;
    if (a.get_end_states().empty()){
        fout << "Results for " << input_regex << ": " << std::endl;
        fout << "Empty regex" << std::endl;
        fout << "===========================================================" << std::endl;
        return;
    }
    get_words(a, col_words);
    boost::regex r;
    if (!lb_flag) r = boost::regex(input_regex);
    boost::regex out_r(output_regex);
    if (!lb_flag) {
        for (const auto &word: words) {
            bool res = boost::regex_match(word, match, r);
            if (res) col_true++;
            else {
                problem_words.push_back(word);
            }
        }
        fout << "Results for " << input_regex << ": " << std::endl;
        fout << "Words true: " << col_true << "/" << col_words << std::endl;
    }
    else{
        fout << "Results for " << input_regex << ": " << std::endl;
        fout << "This is regex with lookbehind with * or |, automaton won't be tested" << std::endl;
    }
    if (problem_words.empty()){
        fout << "OK" << std::endl;
        fout << std::endl;
        fout << "Test output regex " << output_regex << std::endl;
        for (const auto & word : words){
            bool res = boost::regex_match(word, match, out_r);
            if (res) col_true++;
            else{
                problem_words.push_back(word);
            }
        }
        if (problem_words.empty()) {
            fout << col_words << " words checked, " <<  std::endl << "OK" << std::endl;
        }
        else{
            fout << "Problem words for output regex:" << std::endl;
            for (const auto& word : problem_words) {
                fout << "\"" << word << "\"" << std::endl;
            }
        }
    }
    else{
        fout << "Problem words:" << std::endl;
        for (const auto& word : problem_words) {
            fout << "\"" << word << "\"" << std::endl;
        }
    }
    words.clear();
    fout << "===========================================================" << std::endl;
}


