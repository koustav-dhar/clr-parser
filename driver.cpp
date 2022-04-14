#include <bits/stdc++.h>
#include "lexicalanalyser.cpp"
#include "clrparser.cpp"


using namespace std;

int main(){
    cout << "Enter file name of file containing grammar : ";
    string file_name;
    cin >> file_name;
    CLR1Parser cp;
    try{
        cp = CLR1Parser(file_name);
        cout << "Grammar read, item set and parsing table built successfully\n\n";
    }
    catch(exception e){
        cout << "Oops! error occured\n";
        return 0;
    }

    cout << "Enter file name of file containing regular expressions : ";
    cin >> file_name;
    LexicalAnalyser la;
    try{
        la = LexicalAnalyser(file_name);
        cout << "Scanner built successfully\n\n";
    }
    catch(exception e){
        cout << "Oops! error occured\n";
        return 0;
    }


    string input_file_name;
    string inmd_file_name = "intermediate_file.txt";
    bool flag = false;
    int ft = 0;

    while(ft != 2){
        cout << "Input your choice-----\n" << " 1. Verify a input file\n" << " 2. Exit\n" << "\nEnter your choice : ";
        cin >> ft;
        switch(ft){
            case 1:
                cout << "Enter input file name : ";
                cin >> input_file_name;
                la.scan(input_file_name, inmd_file_name);
                cp.parse(inmd_file_name);
                break;
            default:
                ft = 2;
                break;
        }
    }
}