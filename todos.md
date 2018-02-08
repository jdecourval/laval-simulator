# Required for CSGames 2018
- Finish documentation
- Add a couple of tests/challenges
- Produce some figures to help documentation
- Test I/O
    * Should be impossible to wire I/O to cores not on an edge
- Deadlock detector (max number of cycles)

# Optional for CSGames 2018
- Check with the undefined behaviour sanitizer and valgrind
- Remove wrap
- Merge similar tests under the sames test cases
- Consider adding other logical operators
- Consider adding AND and OR instructions affecting the high part

# Nice to have, maybe one day
- Run tests on ARM
- As most older 8 bits CPU, consider if using some 16 bits registers could prove useful
- Pipelining and prefetching
- Fix HLT when using a thread pool
    Right now, thread pool must be disabled to make this instruction work. Not a huge deal.

# Tests/challenges idea
- Simple subtraction
- Sum with multiple numbers
- Logarithm (other than base 2) $ log_2 x / log_2 base $
- Common divisor
- popcnt
- 16 bits add
- Float
