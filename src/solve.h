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
    link  *out;

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
    void pretty_print(char* );
    void print_solution(char* );

    // to solve a particular puzzle
    void cover_colns(char* );
    void restore_colns();
    void solve_puzzle(char* );
    void solve_puzzle(std::ifstream& , bool quiet = true,
                      bool warn_multiple = true);

    bool quiet = false;

protected:
    us solutions = 0;
    node    solution[INFTY];
    char    solution_str[81]; // To be printed

private:
    us max_solns = 2;

    // variables to parse the toroidal quadruply linked list
    node    root;
    node    this_head;
    node    tmp_head;
    node    tmp_row;

    node    coln_headers[COLNS];

    // other variables
    ul      min_size;
    node    min_coln;


};

#endif // SOLVE_H
