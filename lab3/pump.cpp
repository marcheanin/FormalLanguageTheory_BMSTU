#include "automaton.cpp"

class pump{
private:
    std::vector<std::string> ways_to_cycle;
    std::vector<std::string> vertex_cycles;
    std::vector<std::string> ways_from_cycle;
    int vertex_num;
    std::vector<std::string> pumps;
public:
    pump(int n, automaton auto1);
    std::vector<std::string> get_ways_to_cycle();
    std::vector<std::string> get_vertex_cycles();
    std::vector<std::string> get_ways_from_cycle();
    void print_pump();
};
pump::pump(int n, automaton auto1) {
    ways_to_cycle = auto1.get_all_ways_to_all_vertexes()[n];
    vertex_cycles = auto1.get_cycles()[n];
    ways_from_cycle = auto1.get_all_ways_from_all_vertexes()[n];
    std::cout << "Built pump for vertex " << n << std::endl << "Ways to vertex count = " << ways_to_cycle.size() << std::endl << "Cycles count = " << vertex_cycles.size() << std::endl << "Ways from vertex count = " << ways_from_cycle.size() << std::endl;
    vertex_num = n;
    for (const auto &to : ways_to_cycle){
        for (const auto &cycle : vertex_cycles){
            for (const auto &from : ways_from_cycle){
                pumps.push_back(to + cycle + from);
            }
        }
    }
}

std::vector<std::string> pump::get_ways_to_cycle() {
    return ways_to_cycle;
}

std::vector<std::string> pump::get_vertex_cycles() {
    return vertex_cycles;
}

std::vector<std::string> pump::get_ways_from_cycle() {
    return ways_from_cycle;
}

void pump::print_pump() {
    std::cout << std::endl << "To vertex: ";
    for (const auto & i : ways_to_cycle){
        std::cout << i << " ";
    }
    std::cout << std::endl << std::endl << "Vertex cycles: ";
    for (const auto & i : vertex_cycles){
        std::cout << i << " ";
    }
    std::cout << std::endl << std::endl << "From vertex: ";
    for (const auto & i : ways_from_cycle){
        std::cout << i << " ";
    }
    std::cout << std::endl;
}


class pumps{
private:
    std::map<int, pump> automaton_pumps;
public:
    pumps(automaton a);
    std::map<int, pump> get_automaton_pumps();

};

pumps::pumps(automaton a) {
    for (int i = 0; i < a.get_transition_matrix().size(); i++){
        automaton_pumps.insert(std::make_pair(i, pump(i, a)));
    }
}

std::map<int, pump> pumps::get_automaton_pumps() {
    return automaton_pumps;
}


