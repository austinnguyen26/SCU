def readCvs(filepath):
    data = []
    with open(filepath, 'r') as file:
        for line in file.readlines():
            data.append(line.strip().split(','))
    return data

sData = readCvs('students.csv')

print(sorted(sData))
print(sorted(sData, key=lambda x:x[1]))

import csv
def sort_csv(file_path):
    with open(file_path, newline='') as csvfile:
        reader = csv.reader(csvfile)
        data = list(reader)
    data.sort()
    return data