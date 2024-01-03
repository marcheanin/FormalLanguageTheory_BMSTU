//
// Created by march on 05.10.2023.
//

#include <string>


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

TreeNode* build_tree(const std::vector <std::pair <std::string, std::string > >& postfix) {
    if (postfix.empty()) return nullptr;

    std::stack <TreeNode*> s;
    int c = 1;

    for (const auto& elem : postfix) {
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
                auto* node = new TreeNode(elem, y, x);
                s.push(node);
            }
        }
        else {
            s.push(new TreeNode({elem.first + std::to_string(c), elem.second}, nullptr, nullptr));
            c++;
        }
    }
    return s.top();
}

struct Trunk{
    Trunk *prev;
    std::string str;

    Trunk(Trunk *prev, std::string str)
    {
        this->prev = prev;
        this->str = str;
    }
};

void showTrunks(Trunk *p){
    if (p == nullptr) {
        return;
    }

    showTrunks(p->prev);
    std::cout << p->str;
}

void printTree(TreeNode* root, Trunk *prev, bool isLeft){
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
