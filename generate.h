#ifndef GENERATE_H
#define GENERATE_H

#include "solve.h"

typedef unsigned long ul;

class generate : public solve {

public:
    explicit generate(ul );
    void generate_grid();
    void random_grid(); // Generate a random grid

private:
    char grid[81] = {};
};

#endif // GENERATE_H
