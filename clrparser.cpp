#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include "cfg.cpp"
#include "itemsetbuilder.cpp"

using namespace std;

// ε

class TableEntry{
    public:
    string op_type;
    int new_state_no;
    pair<string, vector<string>> reducing_rule;

    public:
    TableEntry(){
        //Do Nothing
    }

    TableEntry(string op, int n, pair<string, vector<string>> rr){
        op_type = op;
        new_state_no = n;
        reducing_rule = rr;
    }

    TableEntry(string op, int n){
        op_type = op;
        new_state_no = n;
    }

    TableEntry(string op, pair<string, vector<string>> rr){
        op_type = op;
        reducing_rule = rr;
    }
};

class CLR1Parser {
    Grammar G;
    ItemSet item_set;
    vector<unordered_map<string, TableEntry>> parsing_table;

    public: 
    CLR1Parser(string file_name) {
        G = Grammar(file_name);
        item_set = ItemSet(G);
        parsing_table = vector<unordered_map<string, TableEntry>>(item_set.items.size());

        for(EdgeInfo ei : item_set.edge_list){
            if(G.is_non_terminal(ei.symbol))
                parsing_table[ei.from].insert(pair<string, TableEntry>(ei.symbol, TableEntry("goto", ei.to)));
            else
                parsing_table[ei.from].insert(pair<string, TableEntry>(ei.symbol, TableEntry("shift", ei.to)));
        }

        for(Item item : item_set.items){
            for(ProductionLookahead pl : item.prods){
                if(pl.index == pl.rhs.size()){
                    for(string la : pl.look_ahead){
                        parsing_table[item.node].insert(pair<string, TableEntry>(la, TableEntry("reduce", pair<string, vector<string>>(pl.lhs, pl.rhs))));
                    }
                }
            }
        }
    }

    void print_parsing_table(){
        int index = 0;
        for(unordered_map<string, TableEntry> temp : parsing_table){
            cout << "\nTable entry for Item " << index << " -----\n";
            cout << "GOTO Entries\n";
            for(string t : G.terminals){
                if(temp.find(t) != temp.end()){
                    TableEntry te = temp[t];
                    cout << "On symbol : " << t << " ";
                    cout << "Op name : " << te.op_type;
                    if(te.op_type == "shift"){
                        cout << " goto : " << te.new_state_no;
                    }
                    else if(te.op_type == "reduce"){
                        cout << " Reduce : " << te.reducing_rule.first << " -> ";
                        for(string s : te.reducing_rule.second){
                            cout << s << " ";
                        }
                    }
                    cout << "\n";
                }
            }

            cout << "ACTION Entries\n";
            for(string t : G.non_terminals){
                if(temp.find(t) != temp.end()){
                    TableEntry te = temp[t];
                    cout << "On symbol : " << t << " ";
                    cout << "Op name : " << te.op_type;
                    if(te.op_type == "goto"){
                        cout << " goto : " << te.new_state_no;
                    }
                    cout << "\n";
                }
            }
            index++;
        }
    }
    
};

int main() {

    CLR1Parser C = CLR1Parser("input2.txt");
    C.print_parsing_table();
    return 0;
}