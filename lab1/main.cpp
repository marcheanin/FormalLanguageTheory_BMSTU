#include <iostream>
#include <utility>
#include <vector>
#include <cassert>

void print_ord(const std::vector <std::string>& a){
    for (int i = a.size() - 1; i >= 0; i--) {
        if (i != 0) {
            std::cout << "w^" << char('0' + i) << "*";
        }
        std::cout << a[i];
        if (i != 0) std::cout << " + ";
    }
}

void print_func(std::pair <std::vector <std::string>, std::vector <std::string> > f) {
    std::cout << "( ";
    print_ord(f.first);
    std::cout << " )x + ";
    print_ord(f.second);
}

std::vector <std::pair <std::vector <std::string>, std::vector <std::string> > > generate_func_vector(const std::string& input){
    std::vector <std::pair <std::vector <std::string>, std::vector <std::string> > > funcs (input.size());

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

        funcs[i].first.emplace_back(a0), funcs[i].first.push_back(a1);
        funcs[i].second.emplace_back(b0), funcs[i].second.push_back(b1);

        print_func(funcs[i]);
        std::cout << std::endl;
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

//g(f) = W1g * W1f + W1g * W2f + W2g
std::pair <std::vector <std::string>, std::vector <std::string> > composition (const std::pair <std::vector <std::string>, std::vector <std::string> >& f,
                                                                               const std::pair <std::vector <std::string>, std::vector <std::string> >& g) {
    std::pair <std::vector <std::string>, std::vector <std::string> > res;
    res.first = mult_ords(g.first, f.first);
    res.second = sum_ords(mult_ords(g.first, f.second), g.second);

    return res;
}

int main() {
    std::vector <std::string> ord1 ({"(a0)", "(a1)", "(a2)", "(a3)"});
//    //std::vector <std::string> ord2 ({5, 6, 7, 8});
//    auto res = mult_ords(ord1, "(c0)", "(c1)");
//    print_ord(res);
//    print_ord(ord1);

    std::string input;
    std::cin >> input;

    std::vector <std::pair <std::vector <std::string>, std::vector <std::string> > > funcs = generate_func_vector(input);
    std::pair <std::vector <std::string>, std::vector <std::string> > c = composition(funcs[0], funcs[1]);

    print_func(c);

}
