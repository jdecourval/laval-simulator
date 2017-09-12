total_space = 2**8

ops = {'NOP':1, 'SYN':1, 'CTC': 1, 'CTV': 1, 'DBG':1, 'HCF': 1, 'MXL':2, 'MXA':2, 'MXS': 2, 'LMX': 4, 'HMX': 4, 'LCL': 4, 'LCH': 4, 'JLV': 4, 'JEV': 4, 'JGV': 4, 'JMP': 4, 'LCS':5, 'RCS': 5, 'CAD': 5, 'CSU': 5}
space_used = sum([i**2 for i in ops.values()])

print("space used:", space_used, "/", total_space)

if space_used > total_space:
    print("Error: instructions take too much space")

current = total_space - 1
for key, value in ops.items():
    print(key, bin(current))
    current -= value
