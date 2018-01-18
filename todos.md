# Required for CSGames 2018
- Fix HLT when using a thread pool
    Right now, thread pool must be disabled to make this instruction work. Not a huge deal.
- Convert assertions to proper error messages
    Without this, simulator may crash on improper input with unclear errors.
- Finish documentation
- Add a couple of tests/challenges
- Produce some figures to help documentation

# Optional for CSGames 2018
- Output program size
- Figure out how to return an array of data
    - Maybe by having a global halt instruction
- Merge similar tests under the sames test cases
- Consider adding other logical operators
- Consider adding AND and OR instructions affecting the high part
- Consider vector loading
- Consider multiple parallel bytes return
- Consider multiple serial bytes return (maybe RET or OUT, write to stderr)
- Consider multiple serial bytes input (maybe REA OR INP, read from stdin)

# Nice to have, maybe one day
- Run tests on ARM
- As most older 8 bits CPU, consider if using some 16 bits registers could prove useful
- Check with the undefined behaviour sanitizer
- Pipelining and prefetching

# Tests/challenges idea
## Possible with current architecture
- Simple subtraction
- Sum with multiple numbers
- Division
- Logarithm (other than base 2) $ log_2 x / log_2 base $
- Common divisor
- popcnt

## Need additional features
- 16 bits add
- Float
