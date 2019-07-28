# Custom grader for CEOI 2016 icc problem.
The problem can be found [here](https://oj.uz/problem/view/CEOI16_icc).
Because there is no grader for up-solvers on the sites, I decided to write my own.

## Usage
You just need to implement your `run(int n)` function in the file `icc.cpp`.
You can use the `compile` script in this folder to compile the grader.

The grader take input from STDIN with the following format:
- The first line contains 2 positive integers: `N` - the number of cities,
  and `M` - the maximum number of queries.
- N - 1 lines below contains 2 positive integers not greater than N indicating the road
  in the order that must be accepted by the `setRoad()` function. Note that the road 
  must not form a cycle, as mentioned in the statement.

Beside checking if your `run()` function is OK or not, the grader also checks 
the input format.

## Test generator
I also included a small test generator. I can compile it without compiling the grader.
It will take arguments from the command line and print the test to the STDOUT.
To print the test to a file, please use pipeline. It is also convenience to pipe the test
from the test generator to the grader.
### Usage for test generator
```
./test-generator N [M]
```
Arguments:
- N: the number of cities. Required.
- M: the number of queries. Default: 1625.

## Scripts
There are 3 scripts included:
- `compile`: compiles the grader.
- `run`: runs the grader with the input test file is `test.inp`.
- `test`: run the grader 10000 times with random generated test. Note that the test generator must
  also be compiled. The number of tests, as well as the input for the test generator can be adjusts
  by editing the file.

## Notes
This grader is **deterministic**. That is, the test is the same for every input.
This allows randomized solution to get AC. But the real grader - the one used on oj.uz -
actually is **adaptive**. That means it will tried to maximize the number of queries, bases
on the previous queries made by the solution. This is also mentioned in the
[official editorial](http://www.ceoi2016.ro/competition/tasks/) (see the solution column, the 3rd batch).
Implementing such grader is actually hard. You can also see the official grader in the same page
as the editorial. But I think it would be hard to use. I don't even know it exists before I wrote this
grader.

Anyway, hope these pieces of code will be useful for someone.
