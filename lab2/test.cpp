//
// Created by march on 09.10.2023.
//

std::random_device rd;
std::mt19937 gen(rd());

std::map <int, std::string> ops = {{0, "|"},
                                   {1, CONCAT_OP},
                                   {2, "?="},
                                   {3, "*"}};
int random_value(int low, int high){
    std::uniform_int_distribution<> dist(low, high);
    return dist(gen);
}

std::string regex_gen(int terms_range, int terms_col, int la_num, int star_deep){
    if (terms_col <= 0) terms_col = 1;
    std::string s;
    if (terms_col == 1 || terms_col == 2) {
        int c = random_value(0, terms_range);
        s += char('a' + c);
        return s;
    }
    int range = 3;
    if (la_num == 0 && star_deep == 0) range-= 2;
    else if (la_num == 0 || star_deep == 0) range -= 1;
    int rand_term = random_value(0, range);
    if (range == 2 && rand_term == 2 && star_deep != 0){
        rand_term = 3;
    }
    std::string op = ops[rand_term];
    if (op == "*"){
        return "(" + regex_gen(terms_range, terms_col, 0, star_deep - 1) + ")*";
    }
    if (op == CONCAT_OP){
        return regex_gen(terms_range, terms_col / 2 + 1, la_num, star_deep) + regex_gen(terms_range, terms_col / 2 - 1, la_num, star_deep);
    }
    if (op == "|") {
        return regex_gen(terms_range, terms_col / 2 + 1, la_num, star_deep) + "|" + regex_gen(terms_range, terms_col / 2 - 1, la_num, star_deep);
    }
    std::string symb;
    if (random_value(0, 1)) symb = "$";
    return "(?=" + regex_gen(terms_range, terms_col / 3 + 1, 0, star_deep) + symb + ")" +
           regex_gen(terms_range, terms_col / 3 * 2 - 1, la_num - 1, star_deep);
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

void test_automaton(automaton a, const std::string& input_regex, const std::string& output_regex, int col_words, std::ostream& fout){
    std::vector <std::string> problem_words;
    int col_true = 0;
    if (a.get_end_states().empty()){
        fout << "Results for " << input_regex << ": " << std::endl;
        fout << "Empty regex" << std::endl;
        fout << "===========================================================" << std::endl;
        return;
    }
    get_words(a, col_words);
    std::regex r(input_regex);
    std::regex out_r(output_regex);

    for (const auto & word : words){
        bool res = regex_match(word, r);
        if (res) col_true++;
        else{
            problem_words.push_back(word);
        }
    }
    fout << "Results for " << input_regex << ": " << std::endl;
    fout << "Words true: " << col_true << "/" << col_words << std::endl;
    if (problem_words.empty()){
        fout << "OK" << std::endl;
        fout << std::endl;
        fout << "Test output regex " << output_regex << std::endl;
        for (const auto & word : words){
            bool res = regex_match(word, out_r);
            //std::cout << word << " " << res << std::endl;
            if (res) col_true++;
            else{
                problem_words.push_back(word);
            }
        }
        if (problem_words.empty()) {
            fout << "OK" << std::endl;
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


