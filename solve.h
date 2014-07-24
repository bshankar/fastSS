#ifndef SOLVE_H
#define SOLVE_H

#include "matrix.h"
#include <fstream>

typedef unsigned short us;
typedef unsigned int ui;

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
    void init_dlx();
    void choose_coln();
    void cover(node );
    void uncover(node );
    void search(ui );
    void print_solution();
    void print_solution(char* );

    // to solve a particular puzzle
    void cover_colns(char* );
    void restore_colns();
    void solve_puzzle(char* );
    void solve_puzzle(std::ifstream& , bool quiet);

protected:
    bool quiet = false;
    us cc_index = 0;
    us covered_colns[COLNS] = {};
    us solutions;

    node    solution[INFTY];
    char    solution_str[81]; // To be printed

private:
    struct solve_vars;
    solve_vars *sV;


};

#endif // SOLVE_H
