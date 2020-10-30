import math
print('Введите 3 вещественных числа, каждое на новой строке:')

a = float(input())
b = float(input())
c = float(input())

if a == 0 and b == 0:
    print('Выражение не имеет смысла!')
    exit(0)
if a == 0 and b == 0 and c == 0:
    print('Выражение не имеет смысла!')
    exit(0)
if b == 0 and c == 0:
    print('x = 0')
    exit(0)
if b == 0:
    if a > 0 and c < 0:    
        print('x1 =', math.sqrt(-c / a))
        print('x2 =', -math.sqrt(-c / a))
        exit(0)
    if a < 0 and c > 0:        
        print('x1 =',  math.sqrt(-c / a))
        print('x2 =', -math.sqrt(-c / a))
        exit (0)
    if (a > 0 and c > 0) or (a < 0 and c < 0):
        print('Корней нет!')
        exit(0)
if c == 0:
    print('x1 = 0')
    print('x2 =', -(b / a))
    exit(0)
if a == 0:
    print('x =', (c / b))
else:
    d = (b * b) - (4 * a * c)
    if d < 0:
        print('Нет действительных корней.')
    if d == 0:
        print('x =', -b/(2 * a))
    if d > 0:
        print('x1 =', (-b + math.sqrt(d)) / (2 * a))
        print('x2 =', (-b - math.sqrt(d)) / (2 * a))

    