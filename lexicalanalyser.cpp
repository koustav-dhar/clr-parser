#include <iostream>
#include <fstream>
#include <regex>

using namespace std;

class LexicalAnalyser{
    public:
    vector<pair<string, string>> regex_patterns;

    public:
    LexicalAnalyser(){
        //Do nothing
    }

    LexicalAnalyser(string rif){
        FILE *fp = fopen(rif.c_str(), "r");
        if(fp == NULL){
            cout << "Couldn't open file : " << rif << " \n";
            return;
        }

        string curr_str;
        pair<string, string> curr_pair;
        bool is_lhs = true;
        while(!feof(fp)){
            char ch = getc(fp);
            switch(ch){
                case ':':
                    curr_pair.first = curr_str;
                    is_lhs = false;
                    curr_str.clear();
                    break;
                case ' ':
                    break;
                case '\n':
                    if(is_lhs){
                        if(curr_str.empty())
                            break;
                        curr_pair.first = curr_str;
                        curr_pair.second = " $0 ";
                        regex_patterns.push_back(curr_pair);
                    }
                    else if(!is_lhs){
                        if(curr_str.empty()){
                            curr_pair.second = " $0 ";
                            regex_patterns.push_back(curr_pair);
                        }
                        else{
                            curr_pair.second = curr_str;
                            regex_patterns.push_back(curr_pair);
                        }
                    }
                    is_lhs = true;
                    curr_str.clear();
                    break;
                default:
                    curr_str.insert(curr_str.end(), ch);
            }
        }

        fclose(fp);
    }

    bool scan(string ifl, string ofl){
        ifstream fin(ifl);
        if (!fin.is_open()) {
            cerr << "Could not open the file - \'" << ifl << "\'\n";
            return false;
        }
        ofstream fout(ofl);
        if (!fout.is_open()) {
            cerr << "Could not open the file - \'" << ofl << "\'\n";
            return false;
        }

        for(pair<string,string> rg : regex_patterns){
            // cout << "Regex : " << rg.first << "    output : " << rg.second << "\n";
        }

        // Regex Part
        vector<pair<regex, string>> all_regex;
        for(pair<string,string> rg : regex_patterns){
            all_regex.push_back(pair<regex, string>(regex(rg.first), " "+rg.second+" "));
        }

        regex re_space("[\\s]+");

        // File Reading Line by Line
        string line;
        string output = "";
        while (getline(fin, line)) {
            for(pair<regex, string> curr : all_regex){
                line = regex_replace(line, curr.first, curr.second);
            }
            output += line;
        }
        output = regex_replace(output, re_space, " ");
        fout << output;

        fin.close();
        fout.close();

        return true;
    }
};


// int main(){
//     LexicalAnalyser la("lex_inp.txt");
//     la.scan("code.in", "code.out");
// }
// int main(int argc, char **argv) {
//     if (argc < 2) {
//         cerr << "Enter a filename!!!\n";
//         return EXIT_FAILURE;
//     } else {
//         ifstream fin(argv[1]);
//         if (!fin.is_open()) {
//             cerr << "Could not open the file - \'" << argv[1] << "\'\n";
//             return EXIT_FAILURE;
//         }
//         string output_filename = "lex.out";
//         if (argc == 3) {
//             output_filename = argv[2];
//         }
//         ofstream fout(output_filename);
//         if (!fout.is_open()) {
//             cerr << "Could not open the file - \'" << argv[1] << "\'\n";
//             return EXIT_FAILURE;
//         }


//         // Regex Part
//         string keywords = "int|float|void|main|if|else|return|read|write";
//         string pattern_id = "(\\b(?!" + keywords + "\\b)[A-Za-z_][A-Za-z0-9_]*\\b)";
//         regex re_id(pattern_id);
//         regex re_type("int|float");
//         regex re_void("void");
//         regex re_int("[0-9]+");
//         regex re_float("[0-9]+[.][0-9]+");
//         regex re_space("[\\s]+");
//         regex re_braces("[(){};]");

//         // File Reading Line by Line
//         string line;
//         string output = "";
//         while (getline(fin, line)) {
//             // Replacing the identifiers and constants using regex
//             line = regex_replace(line, re_id, "id");
//             // line = regex_replace(line, re_type, "TYPE");
//             // line = regex_replace(line, re_void, "VOID_TYPE");
//             line = regex_replace(line, re_float, "float");
//             line = regex_replace(line, re_int, "int");
//             line = regex_replace(line, re_braces, " $0 ");
//             // line = regex_replace(line, re_space, " ");
//             // Writing in the output file
//             output += line;
//         }
//         output = regex_replace(output, re_space, " ");
//         fout << output;

//         fin.close();
//         fout.close();

//     }
//     return 0;
// }