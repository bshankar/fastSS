#include "generate.h"
#include <iostream>
#include <cstring>

using namespace std;

void print_help() {
    cout << "========================================" << endl;
    cout << "            fastSS v1.0" << endl;
    cout << "========================================" << endl;
    cout << "Generate puzzles: (default <= 26 clues)" << endl;
    cout << "$ ./sudoku -g <number>" << endl;
    cout << "Generate puzzles with n clues (n <= 17)" << endl;
    cout << "$ ./sudoku -g <number> <clues>" << endl;
    cout << "Solve a puzzle: " << endl;
    cout << "$ ./sudoku <puzzle>" << endl;
    cout << "Solve a text file containing puzzles: " << endl;
    cout << "$ ./sudoku -f <file>" << endl;
    cout << "Fast solve a text file(benchmark): " << endl;
    cout << "$ ./sudoku -fq <file>" << endl;
    cout << "Fast solve a text file(assume unique solution): " << endl;
    cout << "$ ./sudoku -fqa <file>" << endl;
    cout << "print this help: " << endl;
    cout << "$ ./sudoku -h" << endl << endl;
}

int main(int argc, char **argv) {

    if (argc < 2 or argc > 4) {
        print_help();
    }
    else if (argc == 2 and strlen(argv[1]) == 81) {
        solve sol;
        sol.solve_puzzle(argv[1]);
    }

    else if (argc == 3) {

        if (!strcmp(argv[1], "-f")){
            solve sol;
            ifstream file(argv[2]);
            sol.solve_puzzle(file, false);
        }

        else if (!strcmp(argv[1], "-fq")){
            solve sol;
            ifstream file(argv[2]);
            sol.solve_puzzle(file);
        }

        else if (!strcmp(argv[1], "-fqa")){
            solve sol;
            ifstream file(argv[2]);
            sol.solve_puzzle(file, true, false);
        }

        else if (!strcmp(argv[1], "-g")){
            generate gen(atoi(argv[2]));
            gen.generate_grids();
        }
        else
            print_help();
    }

    else
        print_help();
}
