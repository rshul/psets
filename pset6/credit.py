import cs50

print("Number: ", end='')
while True:
    num = cs50.get_int()
    if num <= 0:
        print("Retry: ", end='')
    if num > 0:
        break

temp = 1
digits = []
while num > temp:
    digits.append((num // temp) % 10)
    temp *= 10
    
result = [digits[x] if not x % 2 
    else digits[x] * 2 % 10 + digits[x] * 2 // 10  
    for x in range(len(digits)) ]
    
digits.reverse()
if sum(result) % 2 != 0:
    print("INVALID")
elif digits[0] == 4 and (len(digits) == 13 or len(digits) == 16):
    print("VISA")
elif (digits[:2] == [3,7] or digits[:2] == [3,4]) and len(digits) == 15:
    print("AMEX")
elif digits[0] == 5 and (0 < digits[1] < 6) and len(digits) == 16:
    print("MASTERCARD")
else:
    print("INVALID")
        
            
