fastSS
======

A simple and fast sudoku solver and generator written in C++ based on the dancing
links algorithm. Now [reimplemented](https://github.com/bshankar/qss) in rust with improvements.

Speed
=====

55100.txt is a file containing 55100 puzzles of
various difficulty levels. top50000.txt contains
ontains 50000 very difficult puzzles.

fastSS takes 2.62s to solve 55100.txt i.e **@21030 puzzles/s**
     and 3.49s to solve top50000.txt i.e   **@14326 puzzles/s** on a 2.6Ghz intel i7 CPU.

Without uniqueness checking on the same CPU, fastSS can solve 55100.txt
in 2.34s and top50000.txt in 2.75s.

Compiling
=========

    $ qmake fastSS.pro
    $ make

Usage
=====

To solve a puzzle, enter it as a 81 digit string (zeros for empty cells)

    $ ./fastSS <puzzle>

To solve puzzles in a text file,

    $ ./fastSS -f <file>

To solve puzzles without any output,

    $ ./fastSS -fq <file>

To skip checking for uniqueness of solution and not give any output,

    $ ./fastSS -fqa <file>

To generate 'n' puzzles,

    $ ./fastSS -g <number>
