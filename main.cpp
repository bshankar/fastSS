#include "solve.h"
#include <iostream>

using namespace std;

int main() {
    solve sol;
    char p[] = "tarek_pearly_6000.txt";
    ifstream puzzles(p);
    sol.solve_puzzle(puzzles, true);
}

