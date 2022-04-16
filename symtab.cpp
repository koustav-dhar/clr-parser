//bhai idhar tum symbol table structure likhna

#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

class Attr {
    int lineno;
    int level;
    string data_type;
    char id_type;   // 'f' - functions, 'v' - variable
    vector<pair<string, string>> params;  // parameters if function
    public:
    Attr() {

    }
    Attr(string dt, char it, vector<pair<string, string>> p) {
        data_type = dt;
        id_type = it;
        params = p;
    }
    friend class SymTab;
    friend class SemanticAnalyser;
};

class SymTab {
    vector<unordered_map<string,Attr>> hashmaps; 
    int currLevel;
    
    bool checkAtLevel(string idname, int level) {
        if (level <= currLevel) {
            return (hashmaps[level].find(idname) != hashmaps[level].end());
        }
        return false;
    }
    public:
    SymTab() {
        currLevel = -1;
    }
    void entry() {
        ++currLevel;
        hashmaps.push_back(unordered_map<string, Attr>());
    }
    void exit() {
        if (currLevel >= 0) {
            --currLevel;
            hashmaps.pop_back();
        }
    }
    pair<bool, Attr> lookup(string idname) {
        for (int level = currLevel; level >= 0; --level) {
            if (checkAtLevel(idname, level))
                return {true, hashmaps[level].find(idname)->second};
        }
        return {false, Attr()};
    }
    bool insert(string idname, string type, vector<pair<string, string>> params = vector<pair<string, string>>()) {
        if (checkAtLevel(idname, currLevel)) {
            cerr << "Multiple Declarations!!!\n";
            return false;
        } else {
            cout << "hello " << currLevel << "\n";
            hashmaps[currLevel][idname] = Attr(type, (params.empty() ? 'v' : 'f'), params);
            for (auto itr : hashmaps[currLevel]) {
                cout << itr.first << "\n";
            }
            return true;
        }
    }
};


























