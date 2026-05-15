# Part 1:
def get_odd(n):
    return [i for i in range(n + 1) if i % 2 != 0]


print (get_odd(18))
print (get_odd(7))
print (get_odd(21))

# Part 2:
def readCvs(filepath):
    data = []
    with open(filepath, 'r') as file:
        for line in file.readlines():
            data.append(line.strip().split(','))
    return data

sData = readCvs('students.csv')

print(sorted(sData))
print(sorted(sData, key=lambda x:x[1]))

# Part 3:
def frequency(str):
    letters ={}
    for char in str:
        letters[char] = letters.get(char, 0) + 1
    return max(letters, key=letters.get)

print(frequency("milkshake")) 
print(frequency("abbcccddddeeeeeffff"))

# Part 4:
import math

print(math.sqrt(16)) 
