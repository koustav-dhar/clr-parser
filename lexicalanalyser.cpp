#include <iostream>
#include <fstream>
#include <regex>

using namespace std;

int main(int argc, char **argv) {
    if (argc < 2) {
        cerr << "Enter a filename!!!\n";
        return EXIT_FAILURE;
    } else {
        ifstream fin(argv[1]);
        if (!fin.is_open()) {
            cerr << "Could not open the file - \'" << argv[1] << "\'\n";
            return EXIT_FAILURE;
        }
        string output_filename = "lex.out";
        if (argc == 3) {
            output_filename = argv[2];
        }
        ofstream fout(output_filename);
        if (!fout.is_open()) {
            cerr << "Could not open the file - \'" << argv[1] << "\'\n";
            return EXIT_FAILURE;
        }


        // Regex Part
        string keywords = "int|float|void|main|if|else|return|read|write";
        string pattern_id = "(\\b(?!" + keywords + "\\b)[A-Za-z_][A-Za-z0-9_]*\\b)";
        regex re_id(pattern_id);
        regex re_type("int|float");
        regex re_void("void");
        regex re_int("[0-9]+");
        regex re_float("[0-9]+[.][0-9]+");
        regex re_space("[\\s]+");
        regex re_braces("[(){};]");

        // File Reading Line by Line
        string line;
        string output = "";
        while (getline(fin, line)) {
            // Replacing the identifiers and constants using regex
            line = regex_replace(line, re_id, "id");
            // line = regex_replace(line, re_type, "TYPE");
            // line = regex_replace(line, re_void, "VOID_TYPE");
            line = regex_replace(line, re_float, "float");
            line = regex_replace(line, re_int, "int");
            line = regex_replace(line, re_braces, " $0 ");
            // line = regex_replace(line, re_space, " ");
            // Writing in the output file
            output += line;
        }
        output = regex_replace(output, re_space, " ");
        fout << output;

        fin.close();
        fout.close();

    }
    return 0;
}