#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <cassert>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <tuple>
#include <boost/range/combine.hpp>
#include <boost/functional/hash.hpp>
#include <boost/variant/get.hpp>
#include "verilog_driver.hpp"

#if defined(ABC_NAMESPACE)
namespace ABC_NAMESPACE
{
#elif defined(__cplusplus)
extern "C"
{
#endif

// procedures to start and stop the ABC framework
// (should be called before and after the ABC procedures are called)
void   Abc_Start();
void   Abc_Stop();

// procedures to get the ABC framework and execute commands in it
typedef struct Abc_Frame_t_ Abc_Frame_t;

Abc_Frame_t * Abc_FrameGetGlobalFrame();
int    Cmd_CommandExecute( Abc_Frame_t * pAbc, const char * sCommand );
void   Abc_UtilsSource( Abc_Frame_t * pAbc );

#if defined(ABC_NAMESPACE)
}
using namespace ABC_NAMESPACE;
#elif defined(__cplusplus)
}
#endif

inline const std::string INPUT  = "INPUT";
inline const std::string OUTPUT = "OUTPUT";

#ifdef __APPLE__
    #define STDFS std::filesystem
#else
    #define STDFS std::experimental::filesystem
#endif

std::string to_lower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
       [](unsigned char c){ return tolower(c); }
    );
    return s;
}

struct Gate;

struct Wire {
    Wire(std::string n = ""): name(n), prevGate(NULL) {}
    ~Wire() {}

    friend std::ostream& operator << (std::ostream& os, const Wire& w) {
        return os << w.name;
    }
    
    std::string name;
    Gate* prevGate;
    std::vector<Gate*> nextGates;
};

struct Gate {
    Gate(std::string n = "", std::string t = ""): name(n), type(t), fanout(NULL) {}
    ~Gate() {}

    friend std::ostream& operator << (std::ostream& os, const Gate& g) {
        os  << to_lower(g.type) << " " << g.name << "(";
        if (g.fanout == NULL) os << "(NULL)";
        else os << *(g.fanout);
        if (g.fanins.empty()) os << ", (NULL)";
        else {
            for (const auto& input: g.fanins) {
                os << ", " << *input;
            }
        }
        return os << ")";
    }
    
    std::string name;
    std::string type;
    Wire* fanout;
    std::vector<Wire*> fanins;
};

class Module: public verilog::ParserVerilogInterface {
public:
    Module () {
        gates[INPUT] = new Gate(INPUT, INPUT);
        gates[OUTPUT] = new Gate(OUTPUT, OUTPUT);
    }
    virtual ~Module() {
        for (auto& gate: gates) {
            delete gate.second;
        }
        for (auto& wire: wires) {
            delete wire.second;
        } 
    }

    void add_module(std::string&& name) {
        std::cout << "Module name = " << name << '\n';
        moduleName = name;
    }

    void add_port(verilog::Port&& port) {
        switch (port.dir) {
        case verilog::PortDirection::INPUT: {
            Gate* tmpG = gates[INPUT];
            Wire* tmpW = NULL;
            
            if (port.beg == -1 && port.end == -1) {
                for (const auto &name: port.names) {
                    if (wires.find(name) == wires.end()) {
                        tmpW = new Wire;
                        tmpW->name = name;
                        wires[name] = tmpW;
                    }
                    else {
                        tmpW = wires[name];
                    }
                    tmpW->prevGate = tmpG;
                    tmpG->fanins.push_back(tmpW);
                    tmpW = NULL;
                }
            }
            else {
                int del = (port.beg > port.end) ? -1 : 1;
                std::string tmpStr = "";
                for (const auto &name: port.names) {
                    for (int i = port.beg; i != port.end + del; i += del) {
                        tmpStr = name + "_" + std::to_string(i);
                        if (wires.find(tmpStr) == wires.end()) {
                            tmpW = new Wire;
                            tmpW->name = tmpStr;
                            wires[tmpStr] = tmpW;
                        }
                        else {
                            tmpW = wires[tmpStr];
                        }
                        tmpW->prevGate = tmpG;
                        tmpG->fanins.push_back(tmpW);
                        tmpW = NULL;
                    }
                }
            }

            // std::cout << "Port: " << port << '\n';
            break;
        }
        case verilog::PortDirection::OUTPUT: {
            Gate* tmpG = gates[OUTPUT];
            Wire* tmpW = NULL;
            
            if (port.beg == -1 && port.end == -1) {
                for (const auto &name: port.names) {
                    if (wires.find(name) == wires.end()) {
                        tmpW = new Wire;
                        tmpW->name = name;
                        wires[name] = tmpW;
                    }
                    else {
                        tmpW = wires[name];
                    }
                    tmpW->nextGates.push_back(tmpG);
                    tmpG->fanins.push_back(tmpW);
                    tmpW = NULL;
                }
            }
            else {
                int del = (port.beg > port.end) ? -1 : 1;
                std::string tmpStr = "";
                for (const auto &name: port.names) {
                    for (int i = port.beg; i != port.end + del; i += del) {
                        tmpStr = name + "_" + std::to_string(i);
                        if (wires.find(tmpStr) == wires.end()) {
                            tmpW = new Wire;
                            tmpW->name = tmpStr;
                            wires[tmpStr] = tmpW;
                        }
                        else {
                            tmpW = wires[tmpStr];
                        }
                        tmpW->nextGates.push_back(tmpG);
                        tmpG->fanins.push_back(tmpW);
                        tmpW = NULL;
                    }
                }
            }

            break;
        }
        default: std::cout << "Port: " << port << '\n'; break;
        }
    }

    void add_net(verilog::Net&& net) {
        switch (net.type) {
        case verilog::NetType::WIRE: {
            std::cout << "Net: " << net << '\n';
            break;
        }
        default:
            std::cout << "Net: " << net << '\n';
            break;
        }
    }

    void add_assignment(verilog::Assignment&& ast) {
        std::string lStr, rStr;
        Wire* lWire, *rWire;

        Gate* tmpG = new Gate;
        tmpG->type = "buf";

        assert(ast.lhs.size() == 1);
        assert(ast.rhs.size() == 1);

        std::visit(NetConcatToStr(lStr), ast.lhs[0]);
        std::visit(NetConcatToStr(rStr), ast.rhs[0]);
        tmpG->name = lStr + "_" + rStr;
        gates[tmpG->name] = tmpG;

        if (wires.find(lStr) == wires.end()) {
            lWire = new Wire;
            lWire->name = lStr;
            wires[lStr] = lWire;
        }
        else {
            lWire = wires[lStr];
        }
        lWire->prevGate = tmpG;
        tmpG->fanout = lWire;

        if (wires.find(rStr) == wires.end()) {
            rWire = new Wire;
            rWire->name = rStr;
            wires[rStr] = rWire;
        }
        else {
            rWire = wires[rStr];
        }
        rWire->nextGates.push_back(tmpG);
        tmpG->fanins.push_back(rWire);

        // std::cout << "Assignment: " << ast << '\n';
    }

    struct NetConcatToStr {
        NetConcatToStr (std::string &str): str(str) {}
        std::string &str;

        void operator() (verilog::NetBit& v) {
            std::stringstream ss;
            ss << v.name << "_" << v.bit;
            str = ss.str();
        }
        void operator() (const verilog::NetBit& v) {
            std::stringstream ss;
            ss << v.name << "_" << v.bit;
            str = ss.str();
        }

        template<typename T>
        void operator() (T& v) {
            std::stringstream ss;
            ss << v;
            str = ss.str();
        }
    };
    void add_instance(verilog::Instance&& inst) {
        Gate* tmpG = new Gate;
        tmpG->type = to_lower(inst.module_name);
        tmpG->name = inst.inst_name;
        gates[inst.inst_name] = tmpG;

        // I don't think that there will be instances with netsize > 1 in ICCAD 2021 Prob A, 
        // so I'll just assert that this is the case
        // feel free to comment out the following three lines if necessary
        for (const auto& name: inst.net_names) {
            assert(name.size() == 1);
        }
        // get<0>: read as std::string
        std::string tmpStr;
        Wire* tmpW;

        // fanout
        std::visit(NetConcatToStr(tmpStr), inst.net_names[0][0]);
        if (wires.find(tmpStr) == wires.end()) {
            tmpW = new Wire;
            tmpW->name = tmpStr;
            wires[tmpStr] = tmpW;
        }
        else {
            tmpW = wires[tmpStr];
        }
        tmpW->prevGate = tmpG;
        tmpG->fanout = tmpW;
        tmpW = NULL;
        
        // fanins
        for (size_t i = 1; i < inst.net_names.size(); i++) {
            std::visit(NetConcatToStr(tmpStr), inst.net_names[i][0]);
            if (wires.find(tmpStr) == wires.end()) {
                tmpW = new Wire;
                tmpW->name = tmpStr;
                wires[tmpStr] = tmpW;
            }
            else {
                tmpW = wires[tmpStr];
            }
            tmpW->nextGates.push_back(tmpG);
            tmpG->fanins.push_back(tmpW);
        }

        // std::cout << *tmpG << std::endl;
    }

    void print_wires(std::ostream& os = std::cout) const {
        os << "\n" << "WIRES\n";
        for (const auto &iter: wires) {
            if (iter.second->prevGate == NULL) os << "(NULL)";
            else os << iter.second->prevGate->name;
            os << " --> ";
            os << iter.second->name << " --> ";
            if (iter.second->nextGates.empty()) os << "(NULL)";
            else {
                for (const auto &nGate: iter.second->nextGates) {
                    os << nGate->name << " ";
                }
            }
            os << "\n";
        }
        os << std::endl;
    }
    void print_gates(std::ostream& os = std::cout) const {
        os << "\n" << "GATES\n";
        for (const auto &iter: gates) {
            os << *iter.second << "\n";
        }
        os << std::endl;
    }
    void print(std::ostream& os = std::cout) const {
        print_wires(os);
        print_gates(os);
    }

    Gate* getGate(const std::string& name) const {
        auto it = gates.find(name);
        return (it != gates.end()) ? it->second : nullptr;
    }

    Wire* getWire(const std::string& name) const {
        auto it = wires.find(name);
        return (it != wires.end()) ? it->second : nullptr;
    }

    void addGate(Gate* g) {
        gates[g->name] = g;
    }

    void addWire(Wire* w) {
        wires[w->name] = w;
    }

    void print_abc_verilog(std::ostream& os = std::cout) {
        bool first_was_print = false;

        os << "module " << moduleName << " (";
        std::vector<Wire*> outputs = gates[OUTPUT]->fanins;
        for (const auto& output: outputs) {
            if (!first_was_print)
                first_was_print = true;
            else
                os << ", ";
            os << *output;
        }
        std::vector<Wire*> inputs = gates[INPUT]->fanins;
        for (const auto& input: inputs) {
            if (!first_was_print)
                first_was_print = true;
            else
                os << ", ";
            os << *input;
        }
        os << ");\n";

        first_was_print = false;
        os << "input ";
        for (const auto& input: inputs) {
            if (!first_was_print)
                first_was_print = true;
            else
                os << ", ";
            os << *input;
        }
        os << ";\n";

        first_was_print = false;
        os << "output ";
        for (const auto& output: outputs) {
            if (!first_was_print)
                first_was_print = true;
            else
                os << ", ";
            os << *output;
        }
        os << ";\n";

        first_was_print = false;
        int count = 0;
        os << "wire ";
        for (const auto &wire: wires) {
            if (count == 10) {
                os << ";\nwire ";
                count = 0;
                first_was_print = false;
            }
            if (!first_was_print)
                first_was_print = true;
            else
                os << ", ";
            os << *(wire.second);

            ++count;
        }
        os << ";\n";

        for (const auto &gate: gates) {
            if (gate.first == OUTPUT || gate.first == INPUT)
                continue;
            
            os  << to_lower(gate.second->type) << " (";
            if (gate.second->fanout == NULL) os << "(NULL)";
            else os << *(gate.second->fanout);
            if (gate.second->fanins.empty()) os << ", (NULL)";
            else {
                for (const auto& input: gate.second->fanins) {
                    os << ", " << *input;
                }
            }
            os << ");\n";
        }

        os << "endmodule\n";
    }

    void print_weight(std::ostream& os = std::cout) {
        for (const auto &wire: wires) {
            os << wire.second->name << " 1\n";
        }
        os << "\n";
    }

private:
    std::string moduleName;
    std::unordered_map<std::string, Wire*> wires;
    std::unordered_map<std::string, Gate*> gates;
};
// std::tuple is un-std-hashable (yeah, wtf), so we need to pass in a hasher class
// in this case I opt for the one in boost
using RectificationPairs = std::unordered_set<
    std::tuple<Wire*, Wire*>, 
    boost::hash<std::tuple <Wire*, Wire*> >
>;



void getEcoTargets(Module& orig, const Module& gold, RectificationPairs& targets) {
    //find target wires and put them into targets

    RectificationPairs candidates, replacing;

    auto insertToTargetsFromFanins = [&](const Gate* g1, const Gate* g2, RectificationPairs& rp) {
        for (auto const& target: boost::combine(g1->fanins, g2->fanins)) {
            #ifdef __APPLE__
                auto owire = boost::get<0>(target);
                auto gwire = boost::get<1>(target);
            #else 
                auto owire = boost::get<0>(target);
                auto gwire = boost::get<1>(target);
            #endif
            while (owire->prevGate->type == "buf") {
                std::cout << *owire << std::endl;
                owire = owire->prevGate->fanins[0];
                // std::cout << "  owire is buffer, skipping..." << std::endl;
            }
            while (gwire->prevGate->type == "buf") {
                std::cout << *gwire << std::endl;
                gwire = gwire->prevGate->fanins[0];
                // std::cout << "  gwire is buffer, skipping..." << std::endl;
            }
            rp.insert(std::make_tuple(owire, gwire));
        }
    };
    auto origOutputs = orig.getGate(OUTPUT);
    auto goldOutputs = gold.getGate(OUTPUT);
    assert(origOutputs != nullptr);
    assert(goldOutputs != nullptr);
    insertToTargetsFromFanins(origOutputs, goldOutputs, candidates);

    while (candidates.size() > 0) {
        std::cout << "\n\n=========================\n# of candidates: "<< candidates.size() << std::endl;
        
        replacing.clear();
        
        for (auto& target: candidates) {
            // have to copy
            auto& [owire, gwire] = target;

            std::cout << "  " << *owire->prevGate << "\n";
            std::cout << "  " << *gwire->prevGate << "\n";

            // Guard: owire or gwire is PI
            if (owire->prevGate->type == INPUT || 
                gwire->prevGate->type == INPUT) {
                std::cout << "  > Reaching PIs       : Removing this pair\n" << std::endl;   
            } 
            else if (owire->prevGate->type == gwire->prevGate->type &&
                     owire->prevGate->fanins.size() == owire->prevGate->fanins.size()) {
                std::cout << "  > Same gate type     : Replacing with fanins\n" << std::endl;
                replacing.insert(target);
            }
            else {
                std::cout << "  > Different gate type: A rectf. pair\n" << std::endl;
                targets.insert(target);
            }
        }

        candidates.clear();

        for (auto const& target: replacing) {
            auto const& [owire, gwire] = target;
            insertToTargetsFromFanins(owire->prevGate, gwire->prevGate, candidates);
        }
    }

    std::cout << "\n\n=========================\nEnd Finding, Final Target size: "<< targets.size() << std::endl;
    std::cout << "  Rectification Pairs:\n";
    for (auto const& target: targets) {
        auto const& [owire, gwire] = target;
        
        std::cout << "    [" << *owire << " <--> " << *gwire << "]\n";
    }
}

void insertEcoTargets_exec(Module& orig, RectificationPairs& targets) {
    //insert target wires into original circuit
    size_t ctr = 0;
    const Gate* outputGate = orig.getGate(OUTPUT);
    const std::string targetPrefix = "t_";
    auto connectGateWire = [](Gate* g, Wire* w) {
        g->fanout = w;
        w->prevGate = g;
    };
    auto connectWireGate = [](Wire* w, Gate* g) {
        w->nextGates.push_back(g);
        g->fanins.push_back(w);
    };
    auto breakInput = [](Wire* w){
        w->prevGate->fanout = nullptr;
        w->prevGate = nullptr;
    };
    for (auto& [owire, _]: targets) {
        // owire: wire(next, prev)
        
        // originally: prev gate --> owire --> next gate
        // want: prevGate -> owire_dummy -> X; target -> buf -> owire(PO) -> otherGate
        if (!orig.getWire(owire->name + "_dummy")) {
            Gate* prev = owire->prevGate;
            Wire* dummy = new Wire(owire->name + "_dummy");
            Wire* target = new Wire(targetPrefix + std::to_string(ctr));
            Gate* buffer = new Gate(target->name + "_buffer", "buf");


            breakInput(owire);
            connectGateWire(prev, dummy);
            connectWireGate(target, buffer);
            connectGateWire(buffer, owire);

            orig.addWire(dummy);
            orig.addWire(target);
            orig.addGate(buffer);

            std::cout << "\nTarget insertion: " << std::endl;
            std::cout << "    Dummy : " << *dummy << std::endl;
            std::cout << "    Buffer: " << *buffer << std::endl;
        } else {
            // no needs to connect anything, the target has already been linked via previous buffer insertion.
            std::cout << "\nTarget duplicate (" << *owire << "). Skipping this pair..." << std::endl;
        }

        ctr++;
    }
}

void insertEcoTargets(Module& orig, const Module& gold) {
    //pass
    RectificationPairs targets;
    getEcoTargets(orig, gold, targets);
    insertEcoTargets_exec(orig, targets);
}


int main( int argc, char * argv[] )
{
    // variables
    Abc_Frame_t * pAbc;
    std::string command;

    //////////////////////////////////////////////////////////////////////////
    // get the input file name
    if (argc != 4) {
        printf( "Usage: ./eco <R1.v> <R2.v> <G1.v>.\n");
        return 1;
    }

    Module orig, gold;
    orig.read(argv[3]); // G1.v
    gold.read(argv[2]); // R2.v
    insertEcoTargets(orig, gold);

    if (!STDFS::is_directory("temp") || 
        !STDFS::exists("temp")) {
        STDFS::create_directory("temp");
    }
    std::ofstream G1("temp/g1.v");
    std::ofstream R2("temp/r2.v");
    std::ofstream weight("temp/weight.txt");
    orig.print_abc_verilog(G1);
    gold.print_abc_verilog(R2);
    orig.print_weight(weight);
    G1.close();
    R2.close();
    weight.close();

    //////////////////////////////////////////////////////////////////////////

    // start the ABC framework
    Abc_Start();
    pAbc = Abc_FrameGetGlobalFrame();
    Abc_UtilsSource( pAbc ); // load alias file "abc.rc"

    command = "runeco temp/g1.v temp/r2.v temp/weight.txt";
    std::cout << command << std::endl;
    if (Cmd_CommandExecute( pAbc, command.c_str() ))
    {
        std::cerr << "Cannot execute command " << command << "." << std::endl;
        return 1;
    }

    // stop the ABC framework
    Abc_Stop();
    return 0;
}

