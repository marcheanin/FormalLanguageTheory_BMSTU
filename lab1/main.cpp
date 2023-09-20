#include <iostream>
#include <utility>
#include <vector>
#include <cassert>
#include <fstream>

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

void print_func(func f) {
    std::cout << "( ";
    print_ord(f.k1);
    std::cout << " )x + ";
    print_ord(f.k2);
}

std::vector <func> generate_func_vector(const std::string& input){
    std::vector <func > funcs (input.size());

    for (int i = 0; i < input.size(); i++){
        std::string a0 = "(", a1 = "(";
        a0.push_back(input[i]), a1.push_back(input[i]);
        a0.push_back('1'), a1.push_back('1');
        a0.push_back('0'), a1.push_back('1');
        a0.push_back(')'), a1.push_back(')');

        std::string b0 = "(", b1 = "(";
        b0.push_back(input[i]), b1.push_back(input[i]);
        b0.push_back('2'), b1.push_back('2');
        b0.push_back('0'), b1.push_back('1');
        b0.push_back(')'), b1.push_back(')');

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
    a[a.size() - 2] += b0;
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

int main() {

    std::ifstream fin1;
    fin1.open("input.txt");
    assert(fin1.is_open());
    std::vector <std::pair <func, func > > rools;
    std::vector <func> funcs1;
    std::vector <func> funcs2;
    std::string l, r;
    while(!fin1.eof()) {
        fin1 >> l >> r >> r;
        std::cout << l << " -> " << r << std::endl;

        funcs1 = generate_func_vector(l);
        funcs2 = generate_func_vector(r);

        func composition1 = apply_compositions(funcs1);
        func composition2 = apply_compositions(funcs2);

        print_func(composition1);
        std::cout << std::endl;
        print_func(composition2);
        std::cout << std::endl;

        rools.emplace_back(composition1, composition2);
    }

//    std::string input;
//    std::cin >> input;
//
//    std::vector <func > funcs = generate_func_vector(input);
//
//    func res = funcs[0];
//    for (int i = 1; i < funcs.size(); i++){
//        res = composition(res, funcs[i]);
//    }
//    print_func(res);
}
