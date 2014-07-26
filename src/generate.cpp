/*
fastSS is a fast sudoku solver and generator written in C++
Copyright (C) 2014 E. Bhavani Shankar

fastSS is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

fastSS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/


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


generate::generate(us n, bool use_pattern,
                   us max_p_elim, us iter) {

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
    us clues = 0;
    int j = 0;
    for(vector<us>::iterator it = cell_order.begin() ; it != cell_order.end(); ++it){
            *it = j++;
    }

    random_device rd;
    mt19937 g(rd());
    shuffle(cell_order.begin(), cell_order.end(), g);

    us pattern_elim = 0;
    char pattern[CELLS] = {};

    if (use_pattern) {
        pattern_elim = max_p_elim;

        uniform_int_distribution<int> distribution(0, hard_patterns.size() - 1);
        us choice = distribution(rd);
        for (us i = 0; i < CELLS; ++i)
            pattern[i] = hard_patterns[choice][i];
    }

    ul max_branches = 0;
    us best_clues = 0;
    char best_puzzle[CELLS] = {};

    for (ui iter_ = 0; iter_ < iter; ++iter_) {
        for (int i = 0; i < CELLS; i++) {

            if (use_pattern and pattern_elim and pattern[cell_order[i]] != '0')
                continue;

            else if (pattern_elim and pattern[cell_order[i]] == '1')
                --pattern_elim;


            char backup = grid[cell_order[i]];
            grid[cell_order[i]] = '0';
            cover_colns(grid);
            clues = cc_index/4;
            branches = 0;
            search(0);

            if ((max_branches < branches and solutions == 1) or !branches) {
                for (us j = 0; j < CELLS; ++j) {
                    best_puzzle[j] = grid[j];
                    max_branches = branches;
                    best_clues = clues;
                }
            }
            if (solutions != 1)
                grid[cell_order[i]] = backup;
            restore_colns();
        }
    }
    quiet = false;
    pretty_print(best_puzzle, best_clues);
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
    branches = 0;
}

