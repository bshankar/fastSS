#ifndef GENERATE_H
#define GENERATE_H

#include "solve.h"

typedef unsigned long ul;

class generate : public solve {

public:
    explicit generate(ul , us max_clues = 26);
    bool generate_grid(us );
    void random_grid(); // Generate a random grid

private:
    char grid[81] = {};
};

#endif // GENERATE_H
