//
// Created by march on 06.12.2023.
//
#include "automaton.cpp"

class Oracle{
public:
    virtual std::string checkEqual(const automaton& input_automaton);
    virtual bool checkMembership(const std::string& word);
};

class AutomatonOracle : public Oracle {
private:
    automaton oracle_automaton;
    automaton prefix_automaton;
    automaton postfix_automaton;
public:
    void setAutomaton(const automaton &input_automaton){
        oracle_automaton = input_automaton;
    }
    
    std::string checkEqual(const automaton &input_automaton) override {}
    bool checkMembership(const std::string &word) override {}
    bool checkPrefixMembership(const std::string  &word) {}
    bool checkPostfixMembership(const std::string  &word) {}

    void buildPrefixAutomaton(); // TODO: Саня Швец
    void buildPostfixAutomaton(); // TODO: Саня швец
};