#ifndef GENERATE_H
#define GENERATE_H

#include "solve.h"

typedef unsigned long ul;

class generate : public solve {

public:
    explicit generate(us n);

    void random_grid(); // Generate a random grid
    void reduce();
    void generate_grid();
    void generate_grids();

private:
    char grid[81] = {};
    us n;
    bool use_pattern;
    us max_p_elim;
    us iter;

};

#endif // GENERATE_H
