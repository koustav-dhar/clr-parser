#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include "cfg.cpp"

using namespace std;

// ε

class CLR1Parser {
    Grammar G;
    unordered_map<string, unordered_set<string>> firstSet;
    unordered_map<string, bool> firstSetBuilt;
    public: 
    CLR1Parser(string file_name) {
        G = Grammar(file_name);
        for (string terminal : G.terminals) {
            firstSet[terminal].insert(terminal);
        }
        firstSet["ε"].insert("ε");
        firstSetBuilt["ε"] = true;
        // buildFirstSet(G.start_symbol);
        for (string nonterminal : G.non_terminals) {
            buildFirstSet(nonterminal);
        }
        showFirstSet();
    }
    void buildFirstSet(string nonterminal) {
        if (G.productions.find(nonterminal) == G.productions.end())
            return;
        for (vector<string> prod : G.productions[nonterminal]) {
            // cout << "\n" << nonterminal << "\n";
            for (string token : prod) {
                // cout << token << "\n";
                if (!firstSetBuilt[token])
                    buildFirstSet(token);
                for (string rhs : firstSet[token]) {
                    firstSet[nonterminal].insert(rhs);
                }
                if (firstSet[token].find("ε") == firstSet[token].end()) {
                    firstSet[nonterminal].erase("ε");
                    break;
                }
            }
        }
        firstSetBuilt[nonterminal] = true;
    }
    unordered_set<string> getFirstSet(vector<string> alpha) {

    }

    

    void showFirstSet() {
        for (auto itr : firstSet) {
            cout << itr.first << " : ";
            for (string s : itr.second) {
                cout << s << ", ";
            } 
            cout << "\n";
        }
    }
};

int main() {

    CLR1Parser C = CLR1Parser("input.txt");
    
    return 0;
}