# Required for CSGames 2018
- Convert assertions to proper error messages
    Without this, simulator may crash on improper input with unclear errors.
- Finish documentation
- Add a couple of tests/challenges
- Produce some figures to help documentation

# Optional for CSGames 2018
- Fix HLT when using a thread pool
    Right now, thread pool must be disabled to make this instruction work. Not a huge deal.
- Output program size
- Figure out how to return an array of data
    - Maybe by having a global halt instruction
- Merge similar tests under the sames test cases
- Consider adding other logical operators
- Consider adding AND and OR instructions affecting the high part
- Consider vector loading
    * One way would be to add an addition CPU parameter defining input "ports"
    * We could then send to that port using stdin and read from that port using regular mux instructions
    * This means only cores on the edge could use ports
    * Cores in corner could still have only one port available by wiring them all together
    * This way, ports could be indexed by their associated core
    * This could replace actual argument scheme
- Consider multiple parallel bytes return
    * One way would be to add an addition CPU parameter defining output "ports"
    * We could then read from that port using stdout and send to that port using regular SYN instruction
    * This means only cores on the edge could use ports
    * Cores in corner could still have only one port available by wiring them all together
    * This way, ports could be indexed by their associated core
    * This could replace actual return scheme

- Consider multiple serial bytes return (maybe RET or OUT, write to stderr)
    * The port technique allows that
- Consider multiple serial bytes input (maybe REA OR INP, read from stdin)
    * The port technique allows that

# Nice to have, maybe one day
- Run tests on ARM
- As most older 8 bits CPU, consider if using some 16 bits registers could prove useful
- Check with the undefined behaviour sanitizer
- Pipelining and prefetching

# Tests/challenges idea
## Possible with current architecture
- Simple subtraction
- Sum with multiple numbers
- Logarithm (other than base 2) $ log_2 x / log_2 base $
- Common divisor
- popcnt

## Need additional features
- 16 bits add
- Float
