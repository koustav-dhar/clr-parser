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
    CLR1Parser(){
        //Do nothing
    }

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

        print_parsing_table("parsing_table.txt");
    }

    void print_parsing_table(string file_name){
        ofstream of(file_name);
        if(!of.is_open()){
            cout << "Couldn't open file : " << file_name << "\n";
            return;
        }
        int index = 0;
        for(unordered_map<string, TableEntry> temp : parsing_table){
            of << "\n\nTable entry for Item " << index << " -----\n";
            of << "GOTO Entries\n";
            for(string t : G.terminals){
                if(temp.find(t) != temp.end()){
                    TableEntry te = temp[t];
                    of << "On symbol : " << t << " ";
                    of << "Op name : " << te.op_type;
                    if(te.op_type == "shift"){
                        of << " goto : " << te.new_state_no;
                    }
                    else if(te.op_type == "reduce"){
                        of << " Reduce : " << te.reducing_rule.first << " -> ";
                        for(string s : te.reducing_rule.second){
                            of << s << " ";
                        }
                    }
                    of << "\n";
                }
            }

            of << "ACTION Entries\n";
            for(string t : G.non_terminals){
                if(temp.find(t) != temp.end()){
                    TableEntry te = temp[t];
                    of << "On symbol : " << t << " ";
                    of << "Op name : " << te.op_type;
                    if(te.op_type == "goto"){
                        of << " goto : " << te.new_state_no;
                    }
                    of << "\n";
                }
            }
            index++;
        }

        of.close();
    }
    
    void parse(string file_name){
        ifstream inp_file(file_name);
        // stringstream buffer;
        // buffer << inp_file.rdbuf();
        // string whole_file = buffer.str();
        vector<string> tokens;
        string token;
        stack<int> state_stack;
        stack<string> pruning_stack;
        state_stack.push(0);
        bool flag = false;

        while(getline(inp_file, token, ' ')){
            if(!token.empty())
                tokens.push_back(token);
        }
        tokens.push_back("$");

        // cout <<"Hello here : " << tokens.size() << "\n";
        int curr_index = 0;
        while(curr_index < tokens.size()){
            if(!pruning_stack.empty() && pruning_stack.top()==item_set.augmented_start_symbol && tokens[curr_index]=="$"){
                flag = true;
                // cout << "Break cause end does not occur at s1-$\n";
                break;
            }

            if(parsing_table[state_stack.top()].find(tokens[curr_index]) == parsing_table[state_stack.top()].end()){
                cout << "Top : " << state_stack.top() << "Current token : " << tokens[curr_index] << "Can't find stack top symbol for current token\n";
                break;
            }
            else{
                TableEntry te = parsing_table[state_stack.top()][tokens[curr_index]];
                if(te.op_type == "shift"){
                    state_stack.push(te.new_state_no);
                    pruning_stack.push(tokens[curr_index]);
                    curr_index++;
                    // cout << "Shift done successfully\n";
                }
                else if(te.op_type == "reduce"){
                    // cout << "Top initially : " << state_stack.top() << "\n";
                    cout << "Rule : " << te.reducing_rule.first << " -> ";
                    for(string curr : te.reducing_rule.second){
                        cout << curr << " ";
                    }
                    cout << "\n";
                    
                    int size = te.reducing_rule.second.size();
                    for(int i=0; i<size; i++){
                        pruning_stack.pop();
                        state_stack.pop();
                    }
                    pruning_stack.push(te.reducing_rule.first);
                    if(te.reducing_rule.first == item_set.augmented_start_symbol)
                        continue;

                    if(parsing_table[state_stack.top()].find(pruning_stack.top()) == parsing_table[state_stack.top()].end()){
                        // cout << "Can't find stack top symbol for current non terminal\n";
                        break;
                    }
                    else{
                        state_stack.push(parsing_table[state_stack.top()][pruning_stack.top()].new_state_no);
                    }
                }
            }
        }

        // cout << "Curr index : " << curr_index << "\n";
        // cout << "Stack top : " << state_stack.top() << "\n";
        // cout << "Stack top : " << pruning_stack.top() << "\n";

        if(flag){
            cout << "\nThe input belongs to this language\n\n";
        }
        else{
            cout << "\nThe input does not belongs to this language\n\n";
        }
    }
};

// int main() {

//     CLR1Parser C = CLR1Parser("input2.txt");
//     C.print_parsing_table();
//     cout << "\n\n";
//     C.parse("my_inp.txt");
//     return 0;
// }