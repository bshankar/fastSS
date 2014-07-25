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


#include "matrix.h"
#include <iostream>

using std::cout;
using std::endl;

typedef unsigned short us;
typedef unsigned int   ui;
typedef unsigned long  ul;


matrix::matrix() {

    for (ui c = 0; c < CELLS; ++c) {
        for (ui k = 0; k < DIGITS; ++k) {

            us row = c*DIGITS + k;

            m[row][c] = 1;
            m[row][c/DIGITS*DIGITS + CELLS + k] = 1;
            m[row][c%DIGITS*DIGITS + 2*CELLS + k] = 1;
            m[row][(c/N - c/DIGITS*N + c/(N*DIGITS)*N)*DIGITS + 3*CELLS + k] = 1;
        }
    }
}


void matrix::print_m() {
    // print m
    for (ui i = 0; i < ROWS; ++i) {
        for (ui j = 0; j < COLNS; ++j) {
            cout << m[i][j] << " ";
        }
        cout << endl;
    }
}

