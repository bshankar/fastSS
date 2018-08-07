#include "generate.h"
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <iterator>

typedef unsigned short us;
typedef unsigned int ui;
typedef unsigned long ul;

using std::cout;
using std::endl;
using std::vector;
using std::to_string;
using std::random_device;
using std::mt19937;
using std::shuffle;
using std::uniform_int_distribution;


generate::generate(us n) {

    quiet = true;

    this->n = n;
    this->use_pattern = use_pattern;
    this->max_p_elim = max_p_elim;
    this->iter = iter;

    for (us i = 0; i < CELLS; ++i)
        grid[i] = '0';
}


void generate::generate_grids() {

    ul puzzles_found = 0;

    while(puzzles_found < n) {
        random_grid();
        generate_grid();
        ++puzzles_found;

        for (us i = 0; i < CELLS; ++i)
            grid[i] = '0';

        for (ul i = 0; i < INFTY; ++i)
            solution[i] = NULL;
    }
}


void generate::generate_grid() {

    // visit each cell in random order
    vector<us> cell_order(CELLS);
    int j = 0;
    for(vector<us>::iterator it = cell_order.begin() ; it != cell_order.end(); ++it){
            *it = j++;
    }

    random_device rd;
    mt19937 g(rd());
    shuffle(cell_order.begin(), cell_order.end(), g);
    char best_puzzle[CELLS] = {};

    for (int i = 0; i < CELLS; i++) {
        char backup = grid[cell_order[i]];
        grid[cell_order[i]] = '0';
        cover_colns(grid);
        search(0);

        if (solutions == 1) {
            for (us j = 0; j < CELLS; ++j) {
                best_puzzle[j] = grid[j];
            }
        }
        if (solutions != 1)
            grid[cell_order[i]] = backup;
        restore_colns();
    }
    quiet = false;
    pretty_print(best_puzzle);
    quiet = true;
}


void generate::random_grid() {

    vector<us> all_digits(DIGITS);

    us j = 0;
    for(vector<us>::iterator it = all_digits.begin() ; it != all_digits.end(); ++it){
            *it = j++;
    }

    random_device rd;
    mt19937 g(rd());
    shuffle(all_digits.begin(), all_digits.end(), g);

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

    for (ul i = 0; i < INFTY; ++i)
        solution[i] = NULL;

    cover_colns(grid);
    search(0);

    for (ui i = 0; i < CELLS; ++i) {
        grid[i] = solution_str[i];
    }

    restore_colns();
}
