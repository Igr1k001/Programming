import random as rand
import time
import math
def BozoSort(mass, des = True):
    result = []
    if type(mass[0]) == list:
        for elem in mass:
            result += elem
    else:
        result = mass.copy()
    n = len(result)
    while(is_sorted(result, des) == False):
        for i in range(n-1):
            k, i = rand.randint(0,n-1), rand.randint(0,n-1)
            result[i], result[k] = result[k], result[i]
    return result
def is_sorted(mass, des):
    result = mass
    n = len(result)
    if des:
        for i in range (n-1):
            if result[i] > result[i+1]:
                return False
        return True
    else: 
        for i in range (n-1):
            if result[i] < result[i+1]:
                return False
        return True
def BozoSortF3(a,b,c,des = True):
    return BozoSort([a,b,c], des)
n = int(input())
n1 = list(map(int, input().split()))
matriza = []
strochka = []
i = 0
for elem in n1:
    strochka.append(elem)
    i +=1
    if i% math.sqrt(n) == 0:
        matriza.append(strochka)
        strochka = []
        i = 0
del strochka, i
del(n1[n::])
if len(n1) < n: print("Error, try again")
print(' '.join(map(str, BozoSort(n1))))
print(' '.join(map(str, BozoSort(n1, False))))
print(' '.join(map(str, BozoSort(matriza))))
print(' '.join(map(str, BozoSort(matriza, False))))
print(' '.join(map(str, BozoSortF3(n1[0],n1[1],n1[2]))))
print(' '.join(map(str, BozoSortF3(n1[0],n1[1],n1[2],False))))
