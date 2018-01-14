# TODO
- Run tests on ARM
- As most older 8 bits CPU, consider if using some 16 bits registers could prove useful
- Check with the undefined behaviour sanitizer
- Output program size
- Fix HLT when using a thread pool
- Figure out how to return an array of data
    - Maybe by having a global halt instruction
- Merge similar tests under the sames test cases
- Convert assertions to proper error messages
- Consider adding other logical operators
- Consider adding AND and OR instructions affecting the high part
- Consider vector loading
- Consider multiple parallel bytes return
- Consider multiple serial bytes return (maybe RET or OUT, write to stderr)
- Consider multiple serial bytes input (maybe REA OR INP, read from stdin)

# Tests/challenges idea
- Float
- 16 bits add
