def frequency(str):
    letters ={}
    for char in str:
        letters[char] = letters.get(char, 0) + 1
    return max(letters, key=letters.get)

print(frequency("milkshake")) 
print(frequency("abbcccddddeeeeeffff"))