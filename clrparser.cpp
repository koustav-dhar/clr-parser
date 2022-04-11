#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include "cfg.cpp"

using namespace std;

// ε

class CLR1Parser {
    Grammar G;

    public: 
    CLR1Parser(string file_name) {
        G = Grammar(file_name);
    }
    
};

int main() {

    CLR1Parser C = CLR1Parser("input.txt");
    
    return 0;
}