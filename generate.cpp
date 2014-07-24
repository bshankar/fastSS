#include "generate.h"
#include <iostream>
#include <vector>
#include <algorithm>

typedef unsigned short us;
typedef unsigned int ui;
typedef unsigned long ul;

using std::vector;
using std::to_string;
using std::random_shuffle;

generate::generate(ul n) {

    quiet = true;

    for (us i = 0; i < CELLS; ++i)
        grid[i] = '0';

    us puzzles_found = 0;

    while(puzzles_found < n) {
        random_grid();

        restore_colns();
        generate_grid();
        ++puzzles_found;
        restore_colns();

        for (us i = 0; i < CELLS; ++i)
            grid[i] = '0';
    }
}

void generate::generate_grid() {

    // visit each cell in random order
    vector<us> cellParseOrder(CELLS);

    us j = 0;
    for(std::vector<us>::iterator it = cellParseOrder.begin() ; it != cellParseOrder.end(); ++it){
            *it = j++;
        }

    std::random_shuffle(cellParseOrder.begin(), cellParseOrder.end());

    for (int i = 0; i < CELLS; i++) {
        char backup = grid[cellParseOrder[i]];
        grid[cellParseOrder[i]] = '0';
        cover_colns(grid);
        search(0);
        if (solutions != 1) {
            grid[cellParseOrder[i]] = backup;
        }
        restore_colns();
    }
    quiet = false;
    print_solution(grid);
    quiet = true;
}


void generate::random_grid() {

    vector<us> all_digits(DIGITS);

    us j = 0;
    for(std::vector<us>::iterator it = all_digits.begin() ; it != all_digits.end(); ++it){
            *it = j++;
    }

    std::random_shuffle(all_digits.begin(), all_digits.end());

    us firstBox[]      = {0, 1, 2, 9, 10, 11, 18, 19, 20};
    us firstRowPart[]  = {3, 4, 5, 6, 7, 8};
    us firstColPart[]  = {27, 36, 45, 54, 63, 72};

    us digitsForColn[] = {1, 2, 4, 5, 7, 8};

    for (int i = 0; i < DIGITS; ++i) {
        grid[firstBox[i]] = all_digits[i] + '0';

        if (i < 6) {
            grid[firstRowPart[i]] = all_digits[i+3] + '0';
            grid[firstColPart[i]] = all_digits[digitsForColn[i]] + '0';
        }
    }
    solution[INFTY] = {};
    cover_colns(grid);
    search(0);

    for (ui i = 0; i < CELLS; ++i) {
        grid[i] = solution_str[i];
    }
}
