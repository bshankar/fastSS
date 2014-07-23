#include "solve.h"
#include "matrix.h"
#include <iostream>
#include <vector>

typedef unsigned short  us;
typedef unsigned int ui;
typedef unsigned long ul;

using std::cout;
using std::endl;
using std::vector;
using std::ifstream;

struct solve::solve_vars {

    us max_solns = 2;
    bool quiet = false;

    // variables to parse the toroidal quadruply linked list
    node    root;
    node    this_head;
    node    tmp_head;
    node    tmp_row;

    node    coln_headers[COLNS];

    // other variables
    ul      min_size;
    node    min_coln;
    node    solution[INFTY];
    char    solution_str[81]; // To be printed
    us      solutions;
    std::vector<us> covered_colns;  // store the order in which colns are covered.
};

solve::solve()
    : sV(new solve_vars()) {
    init_dlx();
}

void solve::init_dlx() {
    sV->root      = new link();
    sV->this_head = new link();

    sV->root->right = sV->this_head;
    sV->this_head->left = sV->root;
    sV->this_head->name = 0;
    sV->this_head->size = DIGITS;
    sV->coln_headers[0] = sV->this_head;

    // create and link all column headers
    for (us i = 1; i < COLNS; ++i) {
        sV->tmp_head = new link();
        sV->tmp_head->name = i;
        sV->tmp_head->size = DIGITS;
        sV->this_head->right = sV->tmp_head;
        sV->tmp_head->left = sV->this_head;
        sV->this_head = sV->tmp_head;
        sV->coln_headers[i] = sV->this_head;
    }

    sV->coln_headers[COLNS-1]->right = sV->root;
    sV->root->left = sV->coln_headers[COLNS-1];

    // store immediately above and left
    node above[COLNS] = {};
    node before[ROWS] = {};

    for (us i = 0; i < ROWS; ++i) {
        for (us j = 0; j < COLNS; ++j) {
            if (m[i][j]) {
                sV->tmp_row = new link();
                sV->tmp_row->name = i;
                sV->tmp_row->coln = sV->coln_headers[j];

                // vertical links
                if (above[j] == NULL) {
                    sV->coln_headers[j]->down = sV->tmp_row;
                    sV->tmp_row->up = sV->coln_headers[j];
                }
                else {
                    above[j]->down = sV->tmp_row;
                    sV->tmp_row->up = above[j];
                }
                above[j] = sV->tmp_row;

                // horizontal links
                if (before[i] != NULL) {
                    before[i]->right = sV->tmp_row;
                    sV->tmp_row->left = before[i];
                }
                before[i] = sV->tmp_row;
            }
        }
    }

    // make colns circular
    for (us i = 0; i < COLNS; ++i) {
        sV->coln_headers[i]->up = above[i];
        above[i]->down = sV->coln_headers[i];
    }

    // make rows circular
    for (us i = 0; i < ROWS; ++i) {
        node f_row = before[i]->left->left->left;
        before[i]->right = f_row;
        f_row->left = before[i];
    }
}

void solve::choose_coln() {

    // Minimize branching
    sV->min_coln = sV->root->right;
    sV->min_size = sV->root->right->size;
    for (node cH = sV->root->right; cH != sV->root; cH = cH->right) {

        if (cH->size < sV->min_size and cH->size > 0) {
            sV->min_coln = cH;
            sV->min_size = cH->size;
        }
    }
}

void solve::cover(node c) {

    c->left->right = c->right;
    c->right->left = c->left;

    for (node  i = c->down; i != c; i = i->down) {
        for (node  j = i->right; j != i; j = j->right) {
            j->down->up = j->up;
            j->up->down = j->down;
            j->coln->size--;
        }
    }
    sV->covered_colns.push_back(c->name);
}

void solve::uncover(node c) {

    // Prevent anomalous uncovering
    if (c->name != sV->covered_colns.back())
        return;

    for (node  i = c->up; i != c; i = i->up) {
        for (node  j = i->left; j != i; j = j->left) {
            j->coln->size++;
            j->up->down = j;
            j->down->up = j;
        }
    }

    c->left->right = c;
    c->right->left = c;
    // assuming uncovering takes place
    // in the exact opposite order
    // Decent assumption for expected behavior
    sV->covered_colns.pop_back();
}


void solve::search(ui k) {

    if (sV->solutions >= sV->max_solns)
        return;

    if (sV->root->right == sV->root) {
        ++sV->solutions;
        if (!sV->quiet)
            print_solution();

        if (sV->solutions >= sV->max_solns) {
            cout << "Multiple solutions exist!" << endl;
            return;
        }
        return;
    }

    choose_coln();
    node c = sV->min_coln;
    cover(c);
    for (node  r = c->down; r != c; r = r->down) {
        //O_k <--- r
        sV->solution[k] = r;

        for (node  j = r->right; j != r; j = j->right) {
            cover(j->coln);
        }

        search(k+1);
        // r <--- O_k
        r = sV->solution[k];
        c = r->coln;
        for (node  j = r->left; j != r; j = j->left) {
            uncover(j->coln);
        }
    }
    uncover(c);
    return;
}


void solve::print_solution() {

    for (us i = 0; sV->solution[i]; ++i) {
        us r = sV->solution[i]->name;
        us cell = r/DIGITS;
        us digit = r%DIGITS + 1;
        sV->solution_str[cell] = digit + 48;
    }


    for (us i = 0; i < CELLS; ++i) {
        if (i % 3 == 0) {
            cout << "| ";
        }
        cout << sV->solution_str[i] << " ";
        if (i % 9 == 8) {
            cout << endl;
        }
        if (i == 26 or i == 53) {
            cout << "-----------------------" << endl;
        }
    }
    cout << endl << endl;
}


void solve::cover_colns(char *puzzle) {

    for (us cell = 0; cell < CELLS; ++cell) {

        if (puzzle[cell] != '0') {
            us d = puzzle[cell] - '1';

            // indices of covered colns
            us c1 = cell;
            us c2 = cell/DIGITS*DIGITS + CELLS + d;
            us c3 = cell%DIGITS*DIGITS + 2*CELLS + d;
            us c4 = (cell/N - cell/DIGITS*N + cell/(N*DIGITS)*N)*DIGITS + 3*CELLS + d;

            cover(sV->coln_headers[c1]);
            cover(sV->coln_headers[c2]);
            cover(sV->coln_headers[c3]);
            cover(sV->coln_headers[c4]);

            sV->solution_str[cell] = puzzle[cell];
        }
    }
}


void solve::restore_colns() {
    while (!sV->covered_colns.empty())
        uncover(sV->coln_headers[sV->covered_colns.back()]);
        if (sV->covered_colns.size() > 1)
            sV->covered_colns.pop_back();
    sV->solutions = 0;
}


void solve::solve_puzzle(char *puzzle) {
    cover_colns(puzzle);
    search(0);
}


void solve::solve_puzzle(ifstream& puzzles, bool quiet) {
    char puzzle[CELLS];
    sV->quiet = quiet;

    if (puzzles.is_open()) {
        while (puzzles.good()) {
            puzzles >> puzzle;
            puzzles.ignore(64, '\n');
            cover_colns(puzzle);
            search(0);
            restore_colns();
        }
    }
}
