#include "ts.cpp"

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
        Follow[term.first] = {};
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
        std::cout << "Empty word accept: " << this->flag << std::endl;
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

    int get_states_count(){
        std::set <std::string> buf;
        std::set <std::string>::iterator iter;
        std::map <std::string, std::set <std::string> >::iterator it;
        for (it = this->Follow.begin(); it != this->Follow.end(); it++){
            buf.insert(it->first);
        }
        return buf.size() + 1; // +1 because of start state exists
    }

    int check_state_number(const std::string& state){
        std::map <std::string, std::set <std::string> >::iterator it;
        int cnt = 1;
        for (it = this->Follow.begin(); it != this->Follow.end(); it++){
            if (state == it->first){
                return cnt;
            }
            cnt += 1;
        }
        return 0;
    }

    automaton ffl_2_glushkov(){
        int states_number = get_states_count();
        std::vector<int> start_states(states_number, 0);
        start_states[0] = 1;

        std::vector<std::vector<std::string>> transition_matrix(states_number, std::vector<std::string>(states_number, "0"));
        for (int i = 0; i < this->First.size(); i++){
            auto state = this->First.begin();
            for(int j=0;j<i;j++){
                state++;
            }
            int index = check_state_number(*state);
            transition_matrix[0][index] = *state;
        }

        std::map <std::string, std::set <std::string> >::iterator it;
        for (it = this->Follow.begin(); it != this->Follow.end(); it++){
            int i = check_state_number(it->first);
            for (int ind = 0; ind < it->second.size(); ind++){
                auto transit_to = it->second.begin();
                for (int z = 0; z < ind; z++){
                    transit_to++;
                }
                int j = check_state_number(*transit_to);
                transition_matrix[i][j] = *transit_to;
            }
        }

        std::vector<int> end_states(get_states_count(), 0);
        if (this->flag){
            end_states[0] = 1;
        } else {
            std::set<std::string>::iterator ite;
            for (ite = this->Last.begin(); ite != this->Last.end(); ite++){
                end_states[check_state_number(*ite)] = 1;
            }
        }
        for (int i = 0; i < transition_matrix.size(); i++){
            for (int j = 0; j < transition_matrix.size(); j++){
                if (transition_matrix[i][j] != "0"){
                    std::string::iterator end_pos = std::remove_if(transition_matrix[i][j].begin(), transition_matrix[i][j].end(),
                                                                   isdigit);
                    transition_matrix[i][j].erase(end_pos, transition_matrix[i][j].end());
                }
            }
        }
        return {start_states, transition_matrix, end_states};
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

