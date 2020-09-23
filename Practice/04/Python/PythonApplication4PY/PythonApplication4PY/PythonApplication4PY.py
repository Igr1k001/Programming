a = int(input('Введите число: '))
b = int(input('Введите число: '))

# С дополнительной переменной 
c = a
a = b
b = c


print('a =', a)
print('b =', b)


# Без дополнительной переменной 
c = int(input('Введите число: '))
d = int(input('Введите число: '))
c, d = d, c

print('c =', c)
print('d =', d)

