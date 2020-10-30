MINUTES_IN_HOUR = 60
print('В первой строке введите время прихода одного человека, а на следующей строке другого человека:')
h1, m1 = map(int, input().split(sep =':'))
h2, m2 = map(int, input().split(sep =':'))
m1 = MINUTES_IN_HOUR * h1 + m1
m2 = MINUTES_IN_HOUR * h2 + m2
if abs(m1 - m2) < 15:
    print('Встреча состоится')
else:
    print('Встреча не состоится')

