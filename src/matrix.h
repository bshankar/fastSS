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


#ifndef MATRIX_H
#define MATRIX_H

#include "data.h"

class matrix {
    /*This class generates the matrix, M(i,j) for a 9x9 sudoku.
     *  In matrix M each row represents a possible number placement and
     * each coln represents a constraint. A Sudoku has 81 cells.
     * In each cell, it is possible to place 9 digits.
     * Therefore, a there are 729 possibilities.
     * The constraints have 4 categories:
     *	 1. Each intersection of a row and column has a digit once
     * 	 1. Each row has a digit only once.
     * 	 2.      column
     * 	 3.      box
     *
     * Each category contains 81 constraints each. Therefore, a sudoku has 81x4 = 324 constraints.
     * We represent the sudoku as a 729x324 sparse matrix.
     */

public:
    matrix();
    void  print_m();

protected:
    bool m[ROWS][COLNS] = {};
};

#endif // MATRIX_H
