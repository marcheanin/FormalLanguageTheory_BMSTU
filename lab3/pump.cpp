#include "automaton.cpp"

class pump{
private:
    std::map<int, std::vector<std::string>> ways_to_cycle;
    std::vector<std::string> vertex_cycles;
    std::map<int, std::vector<std::string>> ways_from_cycle;
public:
    void init_pump(automaton auto1, int n);
};

void pump::init_pump(automaton auto1, int n) {
    this->vertex_cycles = auto1.get_cycles()[n];

}