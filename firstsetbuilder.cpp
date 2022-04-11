#include <bits/stdc++.h>
// #include "cfg.cpp"

using namespace std;

class FirstSetBuilder{
    public:
    unordered_map<string, unordered_set<string>> firstSet;
    unordered_map<string, bool> firstSetBuilt;

    public:
    FirstSetBuilder(Grammar G){
        for (string terminal : G.terminals) {
            firstSet[terminal].insert(terminal);
        }
        firstSet["ε"].insert("ε");
        firstSetBuilt["ε"] = true;

        firstSet["$"].insert("$");
        firstSetBuilt["$"] = true;

        // buildFirstSet(G.start_symbol);
        for (string nonterminal : G.non_terminals) {
            buildFirstSet(G, nonterminal);
        }
        // showFirstSet();
    }

    void buildFirstSet(Grammar G, string nonterminal) {
        if (G.productions.find(nonterminal) == G.productions.end())
            return;
        for (vector<string> prod : G.productions[nonterminal]) {
            if(prod[0] == nonterminal){
                // cout << "First of " << nonterminal << " production is : " << prod[0] << "\n";
                continue;
            }

            // cout << "\n" << nonterminal << "\n";
            for (string token : prod) {
                // cout << token << "\n";
                if (!firstSetBuilt[token])
                    buildFirstSet(G, token);
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

    void showFirstSet() {
        for (auto itr : firstSet) {
            cout << itr.first << " : ";
            for (string s : itr.second) {
                cout << s << ", ";
            } 
            cout << "\n";
        }
    }

    unordered_set<string> getFirstOf(Grammar G, vector<string> alpha){
        unordered_set<string> result = unordered_set<string>();
        for(int i=0;i<alpha.size();i++){
            if(!firstSetBuilt[alpha[i]])
                buildFirstSet(G, alpha[i]);
            for (string rhs : firstSet[alpha[i]]) {
                    result.insert(rhs);
            }
            if (firstSet[alpha[i]].find("ε") == firstSet[alpha[i]].end()) {
                result.erase("ε");
                break;
            }
        }
        return result;
    }
};

// int main() {

//     Grammar G = Grammar("input.txt");
//     FirstSetBuilder fsb = FirstSetBuilder(G);
//     // vector<string> alpha;
//     // alpha.insert(alpha.end(),"A");
//     // alpha.insert(alpha.end(),"$");

//     // unordered_set<string> thisus = fsb.getFirstOf(G, alpha);
//     // cout << "First of (A$) is : ";
//     // for(string curr : thisus){
//     //     cout << curr << " , ";
//     // }
//     // cout << "\n";
//     return 0;
// }