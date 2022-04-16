#include <iostream>
#include "symtab.cpp"
#include <fstream>
#include <sstream>
#include <unordered_set>

using namespace std;

class SemanticAnalyser {
    SymTab S;
    vector<string> tokens;
    public:
    void tokenize(string str) {
        string token;
        istringstream ss(str);
        while (ss >> token) {
            tokens.push_back(token);
        }
    }
    bool functionCallCheck(int &index, Attr a, unordered_set<string> &keywords) { // checks if function call is semantically correct
        int cnt = 0;
        index += 2;
        while (tokens[index] != ")") {
            if (keywords.find(tokens[index]) == keywords.end()) {
                pair<bool, Attr> val = S.lookup(tokens[index]);
                if (val.first == false) {
                    cerr << "Undeclared identifier " << tokens[index] << " used.\n";
                } else if (val.second.id_type == 'f') {
                    string func_name = tokens[index];
                    if (functionCallCheck(index, val.second, keywords) == false) {
                        cerr << "Incorrect call of function " << func_name << " .\n";
                    } else {
                        --index;
                    }
                }
                ++cnt;
            } else if (tokens[index] == "c_int" || tokens[index] == "c_float") {
                ++cnt;
            }
            ++index;
        }
        ++index;
        return (cnt == int(a.params.size()));
    }
    void scan(string filename) {
        ifstream fin(filename);
        if (!fin.is_open()) {
            cerr << "Could not open the file - \'" << filename << "\'\n";
            return;
        }
        string str; 
        getline(fin, str);
        cout << str << "\n";
        tokenize(str);

        unordered_set<string> keywords = {"void", "int", "float", "(", ")", "{", "}", "main", "id", "$", ",", ";", "+", "-", "*", "/", "%", "<", ">", "=", "read", "write", "return", "c_int", "c_float", "++","<=", ">=", "==", "if", "else"};

        int sz = tokens.size(), i = 0;

        // for (int i = 0; i < sz; ++i) {
        //     cout << i << " " << tokens[i] << "\n";
        // }

        S.entry();

        while (i < sz) {
            cout << i << " " << sz << " " << tokens[i] << "\n";
            if (tokens[i] == "int" || tokens[i] == "float" || tokens[i] == "void") {
                if (i + 2 < sz && tokens[i + 2] == ";") {   // declaration
                    S.insert(tokens[i + 1], tokens[i]);
                    i += 3;
                } else {    // function
                    string ret_type = tokens[i], func_id = tokens[i + 1];
                    i += 3;
                    vector<pair<string, string>> params;
                    while (i + 2 < sz && tokens[i + 2] == ",") {
                        // cout << "hello " << i << "\n";
                        params.push_back({tokens[i], tokens[i + 1]});
                        i += 3;
                    }
                    if (tokens[i] != ")")
                        params.push_back({tokens[i], tokens[i + 1]});
                    // cout << "yo\n";
                    S.insert(func_id, ret_type, 'f', params);
                    S.entry();
                    for (auto p : params) {
                        S.insert(p.second, p.first);
                    }
                    if (tokens[i] == ")")
                        i += 2;
                    else
                        i += 4;
                }
            } else if (tokens[i] == "if") {
                i += 2;
                while (tokens[i] != ")") {
                    if (keywords.find(tokens[i]) == keywords.end()) {
                        pair<bool, Attr> val = S.lookup(tokens[i]);
                        if (val.first == false) {
                            cerr << "Undeclared identifier " << tokens[i] << " used.\n";
                        } else if (val.second.id_type == 'f') {
                            string func_name = tokens[i];
                            if (functionCallCheck(i, val.second, keywords) == false) {
                                cerr << "Incorrect call of function " << func_name << " .\n";
                            }
                        }
                    }
                    ++i;
                }
                i += 2;
                S.entry();
            } else if (tokens[i] == "{") {
                S.entry();
                ++i;
            } else if (tokens[i] == "}") {
                S.exit();
                ++i;
            } else if (keywords.find(tokens[i]) == keywords.end()) {
                pair<bool, Attr> val = S.lookup(tokens[i]);
                if (val.first == false) {
                    cerr << i << "\n";
                    cerr << "Undeclared identifier " << tokens[i] <<  " used.\n";
                } else if (val.second.id_type == 'f') {
                    string func_name = tokens[i];
                    if (functionCallCheck(i, val.second, keywords) == false) {
                        cerr << "Incorrect call of function " << func_name << " .\n";
                    }
                }
                ++i;
            } else {
                ++i;
            }
        }
        S.exit();
        cout << "Finished Semantic Analysis.\n";
    }
};