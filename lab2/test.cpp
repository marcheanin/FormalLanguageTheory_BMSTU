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
        return "(" + regex_gen(terms_range, terms_col, la_num, star_deep - 1) + ")*";
    }
    if (op == CONCAT_OP){
        return regex_gen(terms_range, terms_col / 2 + 1, la_num, star_deep) + regex_gen(terms_range, terms_col / 2 - 1, la_num, star_deep);
    }
    if (op == "|") {
        return regex_gen(terms_range, terms_col / 2 + 1, la_num, star_deep) + "|" + regex_gen(terms_range, terms_col / 2 - 1, la_num, star_deep);
    }
    std::string symb;
    if (random_value(0, 1)) symb = "$";
    return "(?=" + regex_gen(terms_range, terms_col / 3 + 1, la_num - 1, star_deep) + symb + ")" +
           regex_gen(terms_range, terms_col / 3 * 2 - 1, la_num - 1, star_deep);
}

