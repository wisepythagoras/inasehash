from random import randrange

alpha = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyzyz-_'
default = [x for x in alpha]
mapping = []

res = []
used = {}

for i in range(len(default)):
    r = randrange(len(default))
    v = default[r]
    default.remove(v)

    res.append("'{}'".format(v))
    mapping.append(v)

    if i % 8 == 0:
        res[-1] = "\n    " + res[-1]

    if i == 63:
        break

print('static char lookup_table[64] =\n{\n' + ', '.join(res) + '\n};\n')

res = []

for i in range(0, 256):
    try:
        value = mapping.index(chr(i))
        res.append('0x' + format(value, '02x'))
    except:
        res.append('0x00')

    if i % 8 == 0:
        res[-1] = "\n    " + res[-1]

print('static byte reverse_lookup_table[256] =\n{' + ', '.join(res) + '\n};')

