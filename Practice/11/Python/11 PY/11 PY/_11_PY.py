print("На первой строке введите число, которое хотите возвести в степень.") 
print("На второй строке введите число, в какую степень возводится первое число.")
a = float(input())
n = int(input())
result = 1.0
for i in range(abs(n)):
    result *= a
if n < 0:
    result = 1 / result
print(result)

