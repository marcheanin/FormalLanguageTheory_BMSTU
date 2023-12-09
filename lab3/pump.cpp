#include "automaton.cpp"

class pump{
private:
    std::vector<std::string> ways_to_cycle;
    std::vector<std::string> vertex_cycles;
    std::vector<std::string> ways_from_cycle;
public:
    pump(int n, automaton auto1);
};

pump::pump(int n, automaton auto1) {
    this->ways_to_cycle = auto1.get_all_ways_to_all_vertexes()[n];
    this->vertex_cycles = auto1.get_cycles()[n];
    this->ways_from_cycle = auto1.get_all_ways_from_all_vertexes()[n];
}