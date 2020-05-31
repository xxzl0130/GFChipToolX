import json

file_list = ['2B14',
             'AGS-30',
             'QLZ-04',
             'AT4-5',
             'AT4-6',
             'BGM',
             'M2-5-0',
             'M2-5-2',
             'M2-6-2']
path = '../GFChipDLX/'
table = []

for file in file_list:
    chips_sum = 0
    chips_count = dict()
    fin = open(path + file + '-result.json', 'r')
    data = json.load(fin)
    fin.close()
    for sln in data:
        for chip in sln:
            ID = chip['ID']
            if ID in chips_count:
                chips_count[ID] += 1
            else:
                chips_count[ID] = 1
            chips_sum += 1
    fout = open(file + '.csv', 'w')
    fout.write('id,percent\n')
    chips_count_sorted = sorted(chips_count.items(), key=lambda d: d[1], reverse=True)
    for chip in chips_count_sorted:
        fout.write("%d,%f\n" % (chip[0], chip[1] / chips_sum))
        table.append([file, chip[0], chip[1] / chips_sum])
    fout.close()

table = sorted(table, key=lambda d: d[2], reverse=True)
fout = open('total.csv', 'w')
fout.write('percent,')
for f in file_list:
    fout.write(f + ',')
fout.write('\n')
for it in table:
    fout.write('%f,' % it[2])
    for i in range(len(file_list)):
        if file_list[i] == it[0]:
            fout.write('%d,' % it[1])
        else:
            fout.write(',')
    fout.write('\n')

