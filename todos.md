# TODO
- Run tests on ARM
- As most older 8 bits CPU, consider if using some 16 bits registers could prove useful
- I/O
    - As of now, the solution is to use standard load instruction as input and the new HLT instruction as output
        This means inputs cannot be updated at runtime, this may not be a problem
- Figure how to assign a membank to a core at boot
    - Have some instruction that return the core ID, then normal comparison could jmp to another bank
        - 8 bits could only index 256 at max
        - Limit to 256 cores ?
    - Have a configuration mode with a different set of instruction at boot
- Double check dumped instructions, there seems to be some missing
- Check with the undefined behaviour sanitizer
- Deadlock detector
- Output program size
- Fix HLT when using a thread pool
- Figure out how to return an array of data
    - Maybe by having a global halt instruction
- Assembler

# Tests/challenges idea
- Float
- 16 bits add
