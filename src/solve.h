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


#ifndef SOLVE_H
#define SOLVE_H

#include "matrix.h"
#include <fstream>

typedef unsigned short us;
typedef unsigned int ui;
typedef unsigned long ul;

typedef struct link {
    link  *left;
    link  *right;
    link  *up;
    link  *down;
    link  *coln;

    us name;
    us size;
}* node;



class solve : public matrix{

public:
    explicit solve();
    ~solve();

    void init_dlx();
    void choose_coln();
    void cover(node );
    void uncover(node );
    void search(ul );
    void print_solution();
    void pretty_print(char* , us );
    void print_solution(char* );

    // to solve a particular puzzle
    void cover_colns(char* );
    void restore_colns();
    void solve_puzzle(char* );
    void solve_puzzle(std::ifstream& , bool quiet = true);

    bool quiet = false;

protected:
    us cc_index = 0;
    us solutions = 0;
    ui branches = 0;
    us covered_colns[COLNS] = {};

    node    solution[INFTY];
    char    solution_str[81]; // To be printed

private:
    struct solve_vars;
    solve_vars *sV;


};

#endif // SOLVE_H
