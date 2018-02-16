#

Scoring

Each challenge is presented with a scoring table.
Each table entry represent

Challenges will be tested with inputs different from the ones given as example in this document.


Objectives may be of different types
- bool: You either get all the point or none at all.
- %: The best competitor get all the point, the worst, none.
Every one else score is given by a linear interpolation between the two extremes.
- pts: The competitor with the most point will win the challenge.

Definitions:

- Number of cycles: Number of cycles to get an output for all the test inputs.
- Resources used: `(number of membanks) x (size of membanks) + cores`



## 1. Value passing (10 pts)

### Restrictions

```
.cores 1, 2, 2
.in 0, 1
.out 2, 3
```

### Description
Move values from the two inputs to the two outputs.

### Example input
Input:
```
235 225
87  128
162 156
242 220
81  46
15  233
192 126
62  79
100 2
140 65
```

Output:
```
235 225
87  128
162 156
242 220
81  46
15  233
192 126
62  79
100 2
140 65
```

### Scoring

| Programs for all inputs | Resources used | Number of cycles | total |
|:--:|:--:|:--:|:--:|
| bool | % | % | pts |
|2 | 3| 3| 100 |

# 2. Simple sum

### Restrictions

```
- Two inputs
- One output
```

### Description
Add values from the two inputs and return the result to the single output.
Output should wrap if it exceed maximum.

### Example input
Input:
```
0   0
0   1
153 108
250 224
16  176
109 18
203 78
143 19
141 71
228 145
```

Output:
```
0
1
5
218
192
127
25
162
212
117
```

# 3. Conditional output

### Restrictions

```
- Two inputs (A, B)
- One output
```

### Description
Output must be 0 when A < B, 1 when A = B and 2 when A > B

### Example input
Input:
```
232 4
222 75
48 48
145 123
71 248
28 64
86 251
43 0
252 196
64 192
```

Output:
```
2
2
1
2
0
0
0
2
2
0
```

# 4. Base 2 logarithm

### Restrictions

```
- Three inputs (A, B, C)
- Three outputs (a, b, b)
```

### Description


### Example input
Input:
```
113
21
201
171
172
94
251
249
97
2
```

Output:
```
113 6
21  4
201 7
171 7
172 7
94  6
251 7
249 7
97  6
2   1
```

# 5. Multiplication
