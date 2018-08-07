#include "solve.h"
#include "matrix.h"
#include <iostream>
#include <ctime>

typedef unsigned short  us;
typedef unsigned int ui;
typedef unsigned long ul;

using std::cout;
using std::endl;
using std::ifstream;

solve::solve() {
  init_dlx();
}


void solve::init_dlx() {
    root      = new link();
    this_head = new link();

    root->right = this_head;
    this_head->left = root;
    this_head->name = 0;
    this_head->size = DIGITS;
    coln_headers[0] = this_head;

    // create and link all column headers
    for (us i = 1; i < COLNS; ++i) {
        tmp_head = new link();
        tmp_head->name = i;
        tmp_head->size = DIGITS;
        this_head->right = tmp_head;
        tmp_head->left = this_head;
        this_head = tmp_head;
        coln_headers[i] = this_head;
    }

    coln_headers[COLNS-1]->right = root;
    root->left = coln_headers[COLNS-1];

    // store immediately above and left
    node above[COLNS] = {};
    node before[ROWS] = {};

    for (us i = 0; i < ROWS; ++i) {
        for (us j = 0; j < COLNS; ++j) {
            if (m[i][j]) {
                tmp_row = new link();
                tmp_row->name = i;
                tmp_row->coln = coln_headers[j];

                // vertical links
                if (above[j] == NULL) {
                    coln_headers[j]->down = tmp_row;
                    tmp_row->up = coln_headers[j];
                }
                else {
                    above[j]->down = tmp_row;
                    tmp_row->up = above[j];
                }
                above[j] = tmp_row;

                // horizontal links
                if (before[i] != NULL) {
                    before[i]->right = tmp_row;
                    tmp_row->left = before[i];
                }
                before[i] = tmp_row;
            }
        }
    }

    // make colns circular
    for (us i = 0; i < COLNS; ++i) {
        coln_headers[i]->up = above[i];
        above[i]->down = coln_headers[i];
    }

    // make rows circular
    for (us i = 0; i < ROWS; ++i) {
        node f_row = before[i]->left->left->left;
        before[i]->right = f_row;
        f_row->left = before[i];
    }
}

inline void solve::choose_coln() {

    // Minimize branching
    min_coln = root->right;
    min_size = root->right->size;
    for (node cH = root->right; cH != root; cH = cH->right) {

        if (cH->size == 1) {
            min_size = 1;
            min_coln = cH;
            break;
        }

        if (cH->size < min_size) {
            min_coln = cH;
            min_size = cH->size;
        }
    }
}

inline void solve::cover(node c) {

    c->left->right = c->right;
    c->right->left = c->left;

    for (node  i = c->down; i != c; i = i->down) {

        node j = i->right;
        node k = j->right;
        node l = k->right;

        j->down->up = j->up;
        k->down->up = k->up;
        l->down->up = l->up;

        j->up->down = j->down;
        k->up->down = k->down;
        l->up->down = l->down;

        --j->coln->size;
        --k->coln->size;
        --l->coln->size;
    }
}


inline void solve::uncover(node c) {

    for (node  i = c->up; i != c; i = i->up) {

        node j = i->left;
        node k = j->left;
        node l = k->left;

        ++j->coln->size;
        ++k->coln->size;
        ++l->coln->size;

        j->up->down = j;
        k->up->down = k;
        l->up->down = l;

        j->down->up = j;
        k->down->up = k;
        l->down->up = l;
    }

    c->left->right = c;
    c->right->left = c;
}


void solve::search(ul k) {

    if (max_solns <= solutions)
        return;

    if (root->right == root) {
        ++solutions;
        print_solution();
        return;
    }

    choose_coln();
    node c = min_coln;
    cover(c);

    for (node  r = c->down; r != c; r = r->down) {
        //O_k <--- r
        solution[k] = r;

        node j_ = r->right; node k_ = j_->right; node l_ = k_->right;
        cover(j_->coln); cover(k_->coln); cover(l_->coln);

        search(k+1);
        // r <--- O_k
        r = solution[k];
        c = r->coln;

        j_ = r->left; k_ = j_->left; l_ = k_->left;
        uncover(j_->coln); uncover(k_->coln); uncover(l_->coln);
    }
    uncover(c);
    return;
}


void solve::print_solution() {

    for (us i = 0; solution[i]; ++i) {
        us r = solution[i]->name;
        us cell = r/DIGITS;
        us digit = r%DIGITS + 1;
        solution_str[cell] = digit + 48;
    }

    if (!quiet)
        pretty_print(solution_str);
}

void solve::pretty_print(char *puzzle) {

    cout << "╔═══╤═══╤═══╦═══╤═══╤═══╦═══╤═══╤═══╗" << endl;

    for (us i = 0; i < CELLS; ++i) {
        if (i % 3 == 0) {
            cout << "║";
        }
        else
            cout << "│";

        if (puzzle[i] != '0')
            cout << " " << puzzle[i] << " ";
        else
            cout << "   ";

        if (i % 9 == 8 and !(i == 26 or i == 53 or i == 80)) {
            cout << "║" << endl << "╟───┼───┼───╫───┼───┼───╫───┼───┼───╢" << endl;
        }
        else if (i == 26 or i == 53) {
            cout << "║" << endl << "╠═══╪═══╪═══╬═══╪═══╪═══╬═══╪═══╪═══╣" << endl;
        }
    }
    cout << "║" << endl << "╚═══╧═══╧═══╩═══╧═══╧═══╩═══╧═══╧═══╝" << endl;
}


void solve::print_solution(char *puzzle) {
    for (us i = 0; i < CELLS; ++i) {
        cout << puzzle[i];
    }
}


void solve::cover_colns(char *puzzle) {

    node tmp = root->out; // immediately linked to root
    for (us cell = 0; cell < CELLS; ++cell) {

        if (puzzle[cell] != '0') {
            us d = puzzle[cell] - '1';

            // indices of covered colns
            us c1 = cell;
            us c2 = cell/DIGITS*DIGITS + CELLS + d;
            us c3 = cell%DIGITS*DIGITS + 2*CELLS + d;
            us c4 = (cell/N - cell/DIGITS*N + cell/(N*DIGITS)*N)*DIGITS + 3*CELLS + d;

            node C1 = coln_headers[c1]; node C2 = coln_headers[c2];
            node C3 = coln_headers[c3]; node C4 = coln_headers[c4];

            cover(C1);  cover(C2);
            cover(C3);  cover(C4);

            solution_str[cell] = puzzle[cell];

            root->out = C4;
            C4->out = C3; C3->out = C2;
            C2->out = C1; C1->out = tmp;
            tmp = C4;
        }
    }
}


void solve::restore_colns() {

    for (node c = root->out; c; c = c->out)
        uncover(c);

    solutions = 0;
    root->out = NULL;
}


void solve::solve_puzzle(char *puzzle) {
    cover_colns(puzzle);
    search(0);
    restore_colns();
}


void solve::solve_puzzle(ifstream& puzzles, bool quiet, bool warn_multiple) {
    this->quiet = quiet;
    std::clock_t begin = std::clock();
    char puzzle[CELLS];

    ul no_of_puzzles = 0;
    ul invalid_puzzles = 0;

    cout << "Started solving puzzles ..." << endl;

    if (puzzles.is_open()) {
        while (puzzles.good()) {
            puzzles >> puzzle;
            puzzles.ignore(64, '\n');
            cover_colns(puzzle);
            if (!warn_multiple)
                max_solns = 1;
            search(0);

            if (solutions != 1)
                ++invalid_puzzles;
            ++no_of_puzzles;

            if (no_of_puzzles % 5000 == 0)
                cout << "Solved " << no_of_puzzles << " puzzles ..." << endl;

            restore_colns();
        }
    }
    std::clock_t end = std::clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout << "Solved " << no_of_puzzles << " sudoku puzzles in " << elapsed_secs << " sec " << endl;
    cout << "Time taken per puzzle: " << elapsed_secs/no_of_puzzles << " sec " << endl;
    if (!warn_multiple)
        cout << "Uniqueness check was off. Some puzzles may have multiple solutions!" << endl;
    else
        cout << "Invalid puzzles: " << invalid_puzzles << endl << endl;
}


solve::~solve() {
    // delete the data structure
    for (us i = 0; i < COLNS; ++i) {
        node r = coln_headers[i]->down;
        node tmp = r->down;
        while (r != coln_headers[i]) {
            delete r;
            r = tmp;
            tmp = tmp->down;
        }
        delete tmp;
    }
    delete root;
}
