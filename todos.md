- Remove wrap
- Test I/O
    * Should be impossible to wire I/O to cores not on an edge
- Deadlock detector (max number of cycles)
- Automatic mem_number and mem_size
- Output statistics after assembling
- Check with the undefined behaviour sanitizer and valgrind
- Merge similar tests under the sames test cases
- Consider adding other logical operators
- Consider adding AND and OR instructions affecting the high part
- Run tests on ARM
- Pipelining and prefetching
- Fix HLT when using a thread pool
    Right now, thread pool must be disabled to make this instruction work. Not a huge deal.

# Tests/challenges idea
- Simple subtraction
- Sum with multiple numbers
- Common divisor
- popcnt
- Float
