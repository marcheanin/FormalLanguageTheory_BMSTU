#include <iostream>
#include <utility>
#include <vector>
#include <cassert>
#include <fstream>
#include <set>
#include <cmath>

struct func {
    std::vector <std::string> k1;
    std::vector <std::string> k2;
};

void print_ord(const std::vector <std::string>& a){
    for (int i = a.size() - 1; i >= 0; i--) {
        if (i != 0) {
            std::cout << "w^" << char('0' + i) << "*";
        }
        std::cout << a[i];
        if (i != 0) std::cout << " + ";
    }
}

void print_func(const func& f) {
    std::cout << "( ";
    print_ord(f.k1);
    std::cout << " )x + ";
    print_ord(f.k2);
}

std::vector <func> generate_func_vector(const std::string& input){
    std::vector <func > funcs (input.size());

    for (int i = 0; i < input.size(); i++){
        std::string a0, a1;
        a0.push_back(input[i]), a1.push_back(input[i]);
        a0.push_back('1'), a1.push_back('1');
        a0.push_back('0'), a1.push_back('1');
        //a0.push_back(')'), a1.push_back(')');

        std::string b0, b1;
        b0.push_back(input[i]), b1.push_back(input[i]);
        b0.push_back('2'), b1.push_back('2');
        b0.push_back('0'), b1.push_back('1');
        //b0.push_back(')'), b1.push_back(')');

        funcs[i].k1.emplace_back(a0), funcs[i].k1.push_back(a1);
        funcs[i].k2.emplace_back(b0), funcs[i].k2.push_back(b1);

        //print_func(funcs[i]);
        //std::cout << std::endl;
    }

    return funcs;
}

std::vector <std::string> mult_ords(std::vector <std::string> a, std::vector <std::string> b){
    assert(b.size() == 2);
    assert(a.size() >= 2);

    std::string b0 = b[0], b1 = b[1];
    a.emplace_back("");
    a[a.size() - 1] = b1;
    a[a.size() - 2] += "*" + b0;
    return a;
}

std::vector <std::string> sum_ords(std::vector <std::string> a, std::vector <std::string> b) {
    if (a.size() < b.size()) return b;
    for (int i = a.size() - 1; i >= 0; i--){
        if (i == b.size() - 1) a[i] += "+" + b[i];
        if (i < b.size() - 1) a[i] = b[i];
    }
    return a;
}

//f(g) = W1f * W1g + W1f * W2g + W2f
func composition (const func& f, const func& g) {
    func res;
    res.k1 = mult_ords(f.k1, g.k1);
    res.k2 = sum_ords(mult_ords(f.k1, g.k2), f.k2);

    return res;
}

func apply_compositions(std::vector <func> funcs) {
    func res = funcs[0];
    for (int i = 1; i < funcs.size(); i++){
        res = composition(res, funcs[i]);
    }
    return res;
}

std::string to_polish(const std::string& k){
    std::string res = "(";
    if (k.size() == 7){
        res += "* ";
        res += k.substr(0, 3) + " " + k.substr(4, 3) + ")";
    }
    else if (k.size() == 11){
        res += "+ ";
        res += k.substr(8, 3) + " (* " + k.substr(0, 3) + " " + k.substr(4, 3) + "))";
    }
    else res = k;
    return res;
}

std::string or_and_gen(const std::string& comp_sign, std::vector <std::string> a, std::vector <std::string> b) {
    std::string s;
    for (int i = a.size() - 1; i >= 0; i--){
        if (i > 0)
            s += "(or (" + comp_sign + " " + to_polish(b[i]) + " " + to_polish(a[i]) + ") (and (= " +
                 to_polish(b[i]) + " " + to_polish(a[i]) + ") ";
        else {
            s += "(" + comp_sign + " " + to_polish(b[i]) + " " + to_polish(a[i]) + ")";
            for (int j = 0; j < (a.size() - 1) * 2; j++) s += ")";
        }
    }
    return s;
}

std::string and_gen(std::vector <std::string> a, std::vector <std::string> b) {
    std::string s = "(and";
    for (int i = 0; i < a.size(); i++){
        s += " (= " + to_polish(a[i]) + " " + to_polish(b[i]) + ")";
    }
    s += ")";
    return s;
}

void top_up_with_zeros(std::vector <std::string> &a, std::vector <std::string> &b){
    std::string res;
    for (int i = 0; i < std::max(a.size(), b.size()); i++){
        if (i >= a.size()) {
            a.emplace_back("0");
        }
        else if (i >= b.size()) {
            b.emplace_back("0");
        }
    }
}
//(or (and or_and_compare(<, W1, S1) or_and_compare(<=, W2, S2)) (and and_compare(W1, S1) or_and_compare(<, W2, S2)))
std::string generate_rool_compare(const std::pair <func, func>& rool) {
    std::vector <std::string> W1 = rool.first.k1;
    std::vector <std::string> W2 = rool.first.k2;
    std::vector <std::string> S1 = rool.second.k1;
    std::vector <std::string> S2 = rool.second.k2;

    top_up_with_zeros(W1, S1);
    top_up_with_zeros(W2, S2);

    std::string res = "(assert (or (and " + or_and_gen("<", W1, S1) + " " + or_and_gen("<=", W2, S2) +
                        ") (and " + and_gen(W1, S1) + " " + or_and_gen("<", W2, S2) + ")))\n";
    return res;
}

void generate_smt_file (const std::set <char>& unig, const std::vector <std::pair <func, func > >& rools) {
    std::ofstream fout;
    fout.open("check.smt");
    assert(fout.is_open());
    fout << "(set-logic QF_NIA)\n"
            "(declare-const w1 Int)\n";
    for (auto x : unig) {
        std::string letter; letter.push_back(x);
        std::string a10 = letter + "10", a11 = letter + "11", a20 = letter + "20", a21 = letter + "21";

        fout << "(declare-const " + a10 +  " Int)" << std::endl;
        fout << "(assert (>= " + a10 + " 0))" << std::endl;
        fout << "(declare-const " + a11 +  " Int)" << std::endl;
        fout << "(assert (>= " + a11 + " 0))" << std::endl;
        fout << "(declare-const " + a20 +  " Int)" << std::endl;
        fout << "(assert (>= " + a20 + " 0))" << std::endl;
        fout << "(declare-const " + a21 +  " Int)" << std::endl;
        fout << "(assert (>= " + a21 + " 0))" << std::endl;
    }

    std::string res;
    for (const auto& rool : rools) {
        res += generate_rool_compare(rool);
    }
    fout << res;
    fout << "(check-sat)" << std::endl;
    fout << "(get-model)" << std::endl;
}

int main() {
    std::ifstream fin1;
    fin1.open("input.txt");
    assert(fin1.is_open());
    std::vector <std::pair <func, func > > rools;
    std::vector <func> funcs1;
    std::vector <func> funcs2;
    std::string l, r;
    std::set <char> uniq_func;
    while(!fin1.eof()) {
        fin1 >> l >> r >> r;
        //std::cout << l << " -> " << r << std::endl;

        for (char f : l) uniq_func.insert(f);
        for (char f : r) uniq_func.insert(f);

        funcs1 = generate_func_vector(l);
        funcs2 = generate_func_vector(r);

        func composition1 = apply_compositions(funcs1);
        func composition2 = apply_compositions(funcs2);

//        print_func(composition1);
//        std::cout << std::endl;
//        print_func(composition2);
//        std::cout << std::endl;

        rools.emplace_back(composition1, composition2);
    }

    generate_smt_file(uniq_func, rools);

    return 0;
}

