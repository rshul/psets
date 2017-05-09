import cs50

print("Height: ", end="")
while True:
    n = cs50.get_int()
    if n == 0:
        exit(1)
    if n >= 0 and n <= 23:
        break

for i in range(n):
    print(" " * (n - i - 1), "#" * (i + 1), "  ", "#" * (i + 1),sep='')


