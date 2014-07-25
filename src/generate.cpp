#include "generate.h"
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

typedef unsigned short us;
typedef unsigned int ui;
typedef unsigned long ul;

using std::cout;
using std::endl;
using std::vector;
using std::to_string;
using std::random_shuffle;

generate::generate(ul n, us max_clues) {

    quiet = true;
    ul puzzles_checked = 0;
    cout << "Generating " << n << " puzzles with atmost "
         << max_clues << " clues ..." << endl;

    for (us i = 0; i < CELLS; ++i)
        grid[i] = '0';

    ul puzzles_found = 0;

    while(puzzles_found < n) {
        random_grid();

        if(generate_grid(max_clues))
            ++puzzles_found;

        ++puzzles_checked;
        for (us i = 0; i < CELLS; ++i)
            grid[i] = '0';

        for (ul i = 0; i < INFTY; ++i)
            solution[i] = NULL;
    }
    cout << "Finished generating " << n << " puzzles." << endl;
    cout << "Total " << puzzles_checked << " puzzles were checked." << endl;
}


bool generate::generate_grid(us max_clues) {

    // visit each cell in random order
    vector<us> cell_order(CELLS);
    us clues = CELLS;
    int j = 0;
    for(vector<us>::iterator it = cell_order.begin() ; it != cell_order.end(); ++it){
            *it = j++;
        }

    random_shuffle(cell_order.begin(), cell_order.end());

    for (int i = 0; i < CELLS; i++) {
        char backup = grid[cell_order[i]];
        grid[cell_order[i]] = '0';
        --clues;
        cover_colns(grid);
        search(0);
        if (solutions != 1) {
            grid[cell_order[i]] = backup;
            ++clues;
        }
        restore_colns();
    }
    if (max_clues < clues)
        return false;

    quiet = false;
    print_solution(grid);
    quiet = true;
    return true;
}


void generate::random_grid() {

    vector<us> all_digits(DIGITS);

    us j = 0;
    for(vector<us>::iterator it = all_digits.begin() ; it != all_digits.end(); ++it){
            *it = j++;
    }

    random_shuffle(all_digits.begin(), all_digits.end());

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
