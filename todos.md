# TODO
- Run tests on ARM
- As most older 8 bits CPU, consider if using some 16 bits registers could prove useful
- I/O
    - As of now, the solution is to use standard load instruction as input and the new HLT instruction as output
        This means inputs cannot be updated at runtime, this may not be a problem
- Check with the undefined behaviour sanitizer
- Deadlock detector
- Output program size
- Fix HLT when using a thread pool
- Figure out how to return an array of data
    - Maybe by having a global halt instruction
- Merge similar tests under the sames test cases

# Tests/challenges idea
- Float
- 16 bits add
