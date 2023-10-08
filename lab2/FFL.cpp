class FFL{
public:
    std::set <std::string> First{};
    std::map <std::string, std::set <std::string> > Follow{};
    std::set <std::string> Last{};
    bool flag = false;

    FFL() = default;

    explicit FFL(const std::pair <std::string, std::string>& term) {
        assert(term.second == "TERM");
        First.insert(term.first);
        Last.insert(term.first);
        //Follow[term.first] = {};  //кажется это не нужно
    }

    void show(){
        std::cout << "First: {";
        for (const auto& elem : this->First){
            std::cout << elem << " ";
        }
        std::cout << "}\n";

        std::cout << "Last: {";
        for (const auto& elem : this->Last){
            std::cout << elem << " ";
        }
        std::cout << "}\n";

        for(const auto& elem :  this->Follow){
            std::cout << "Follow(" << elem.first << ") = {";
            for (const auto& i : elem.second) {
                std::cout << i << " ";
            }
            std::cout << "}\n";
        }
        std::cout << std::endl;
    }

    void concatenate(const FFL& b){
        for (const auto& elem : this->Last){
            this->Follow[elem].insert(b.First.begin(), b.First.end());
        }

        for (const auto& elem : b.Follow) {
            this->Follow[elem.first] = elem.second;
        }

        if (!b.flag){  // НЕ примнимает пустое слово
            this->Last = b.Last;
            if (this->flag) {
                this->First.insert(b.First.begin(), b.First.end());
            }
        }
        else{
            this->Last.insert(b.Last.begin(), b.Last.end());
            if (this->flag){
                this->First.insert(b.First.begin(), b.First.end());
            }
        }
        if (!b.flag){
            this->flag = false;
        }
    }

    void alternative(const FFL& b){
        for (const auto& elem : b.Follow) {
            this->Follow[elem.first] = elem.second;
        }
        this->First.insert(b.First.begin(), b.First.end());
        this->Last.insert(b.Last.begin(), b.Last.end());
        this->flag = this->flag || b.flag;
    }

    void unary(){
        this->flag = true;
        for (const auto& elem : this->Last){
            this->Follow[elem].insert(this->First.begin(), this->First.end());
        }
    }
};

FFL process_tree(TreeNode* node){
    //TODO: че то сделать с пересечением
    if (node->data.second == "UNARY") {
        auto a = process_tree(node->left);
        a.unary();
        return a;
    }
    if (node->data.first == CONCAT_OP){
        auto a = process_tree(node->left);
        auto b = process_tree(node->right);
        a.concatenate(b);
        return a;
    }
    if (node->data.first == "|"){
        auto a = process_tree(node->left);
        auto b = process_tree(node->right);
        a.alternative(b);
        return a;
    }
    return FFL(node->data);
}