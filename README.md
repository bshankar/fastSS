fastSS
======

A simple and fast sudoku solver and generator written in C++ based on the dancing 
links algorithm.


Speed
=====

55100.txt is a file containing 55100 puzzles of 
various difficulty levels. top50000.txt contains
ontains 50000 very difficult puzzles.

fastSS takes 2.62s to solve 55100.txt i.e **@21030 puzzles/s**
     and 3.49s to solve top50000.txt i.e   **@14326 puzzles/s** on a 2.6Ghz intel i7 CPU.


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
    
To generate 'n' puzzles,

    $ ./fastSS -g <number>
    
    
Terms of use
============

fastSS is free, and distributed under the **GNU General Public License**
(GPL). Essentially, this means that you are free to do almost exactly
what you want with the program, including distributing it among your
friends, making it available for download from your web site, selling
it (either by itself or as part of some bigger software package), or
using it as the starting point for a software project of your own.

The only real limitation is that whenever you distribute fastSS in
some way, you must always include the full source code, or a pointer
to where the source code can be found. If you make any changes to the
source code, these changes must also be made available under the GPL.

For full details, read the copy of the GPL found in the file named
*Copying.txt*
