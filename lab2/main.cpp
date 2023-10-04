#include <iostream>
#include <fstream>
#include <cassert>
#include <utility>
#include <vector>

#include "parser.cpp"

struct TreeNode {
    std::pair<std::string, std::string> data;
    struct TreeNode *left{};
    struct TreeNode *right{};

    TreeNode(){
        this->data = {"",""};
        this->left = nullptr;
        this->right = nullptr;
    }

    TreeNode(const std::pair<std::string, std::string> &data, TreeNode *left, TreeNode *right) {
        this->data = data;
        this->left = left;
        this->right = right;
    }
};

TreeNode* build_rec(std::vector <std::pair <std::string, std::string > > postfix) {
    if (postfix.empty()) return nullptr;

    std::stack <TreeNode*> s;

    for (auto elem : postfix) {
        if (elem.second != "TERM") {
            TreeNode *x = s.top();
            TreeNode *y;
            s.pop();
            if (elem.second == "UNARY"){
                auto* node = new TreeNode(elem, x, nullptr);
                s.push(node);
            }
            else{
                y = s.top();
                s.pop();
                auto* node = new TreeNode(elem, x, y);
                s.push(node);
            }
        }
        else {
            s.push(new TreeNode(elem, nullptr, nullptr));
        }
    }
    return s.top();
}



TreeNode* build_tree(std::vector <std::pair <std::string, std::string > > postfix){
    assert(!postfix.empty());
    TreeNode *root = build_rec(postfix);
    return root;
}

struct Trunk
{
    Trunk *prev;
    std::string str;

    Trunk(Trunk *prev, std::string str)
    {
        this->prev = prev;
        this->str = str;
    }
};

void showTrunks(Trunk *p)
{
    if (p == nullptr) {
        return;
    }

    showTrunks(p->prev);
    std::cout << p->str;
}

void printTree(TreeNode* root, Trunk *prev, bool isLeft)
{
    if (root == nullptr) {
        return;
    }

    std::string prev_str = "    ";
    Trunk *trunk = new Trunk(prev, prev_str);

    printTree(root->right, trunk, true);

    if (!prev) {
        trunk->str = "———";
    }
    else if (isLeft)
    {
        trunk->str = ".———";
        prev_str = "   |";
    }
    else {
        trunk->str = "`———";
        prev->str = prev_str;
    }

    showTrunks(trunk);
    std::cout << " " << root->data.first << std::endl;

    if (prev) {
        prev->str = prev_str;
    }
    trunk->str = "   |";

    printTree(root->left, trunk, false);
}

int main() {
    std::string input_regex;
    std::ifstream fin("input.txt");
    assert(fin.is_open());
    fin >> input_regex;
    std::vector <std::pair <std::string, std::string > > lexemes = lexer(input_regex);

    for (const auto& i : lexemes){
        std::cout << i.first << " ";
    }

    std::vector <std::pair <std::string, std::string > > postfix = to_postfix(lexemes);
    std::cout << std::endl;
    for (const auto& i : postfix){
        std::cout << i.first << " ";
    }
    std::cout << std::endl;

    TreeNode* tree = build_tree(postfix);

    printTree(tree, nullptr, false);
}
