#include <bits/stdc++.h>
#include "lexicalanalyser.cpp"
#include "clrparser.cpp"


using namespace std;

int main(){
    LexicalAnalyser la = LexicalAnalyser();
    la.scan("code.in", "lex.txt");
    CLR1Parser cp = CLR1Parser("input2.txt");
    cp.parse("my_inp.txt");
}

// Have to print item set and parsing table in files while building clr parser
// Parser would be built once but can parse many files...in a while loop scan and parse
// lexical analyser regex-es written in normal file