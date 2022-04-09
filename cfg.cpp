#include <bits/stdc++.h>

using namespace std;

class Grammar{
    private:
        vector<string> non_terminals;
        vector<string> terminals;
        vector<pair<string, vector<vector<string>>>> productions;
        string start_symbol;

    public:
        Grammar(string file_name){
            try{
                FILE *fp = fopen(file_name.c_str(), "r");
                string curr_str, curr_lhs;
                vector<string> curr_col_str;
                vector<vector<string>> curr_rhs;
                int segment_no = 0;
                while(!feof(fp)){
                    char ch = getc(fp);
                    switch(ch){
                        case '%':
                            curr_str.clear();
                            curr_lhs.clear();
                            curr_col_str.clear();
                            curr_rhs.clear();
                            segment_no++;
                            break;
                        case '\n':
                            if(curr_str.empty())
                                continue;
                            if(segment_no == 1)
                                non_terminals.push_back(curr_str);
                            else if(segment_no == 2)
                                terminals.push_back(curr_str);
                            else if(segment_no == 3){
                                curr_col_str.push_back(curr_str);
                                curr_rhs.push_back(curr_col_str);
                                productions.push_back(pair<string, vector<vector<string>>>(curr_lhs, curr_rhs));
                                curr_col_str.clear();
                                curr_rhs.clear();
                            }
                            else if(segment_no == 4)
                                start_symbol = curr_str;
                            curr_str.clear();
                            break;
                        case '-':
                            if(segment_no == 3){
                                curr_lhs = curr_str;
                                curr_str.clear();
                            }
                            break;
                        case ',':
                            if(segment_no == 1){
                                non_terminals.push_back(curr_str); 
                                curr_str.clear();
                            }
                            else if(segment_no == 2){
                                terminals.push_back(curr_str);
                                curr_str.clear();
                            }
                            break;
                        case ' ':
                            if(segment_no == 3){
                                curr_col_str.push_back(curr_str);
                                curr_str.clear();
                            }
                            break;
                        case '|':
                            if(segment_no == 3){
                                curr_col_str.push_back(curr_str);
                                curr_rhs.push_back(curr_col_str);
                                curr_str.clear();
                                curr_col_str.clear();
                            }
                            break;
                        default:
                            curr_str.insert(curr_str.end(), ch);
                            break;
                    }
                }
            }
            catch(exception e){
                cout << "Failed to build the grammar\n";
            }
        }

        void print_grammar(){
            cout << "The Grammar is -----\n";
            for(int i=0; i<productions.size();i++){
                cout << '\'' << productions[i].first << '\'' << " -> ";
                for(int j=0;j<productions[i].second.size();j++){
                    for(int k=0;k<productions[i].second[j].size();k++){
                        cout << '\'' << productions[i].second[j][k] << '\'' << " ";
                    }
                    cout << "| ";
                }
                cout << "\n";
            }
        }
};

int main(){
    Grammar g = Grammar("input.txt");
    g.print_grammar();
}