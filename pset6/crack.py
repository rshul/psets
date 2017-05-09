import sys
import crypt

def crack(val, n):
    LETTERS = 52
    if n == 0 or n > 4:
        return
    ch_case = ord("a") - ord("A")
    resize = [1,1,1,52,52,52,52]
    resize = resize[n-1:3 + n]
    
    psw = ["A"]*4
    
    for i in range(resize[0]):
        psw[3] = chr(i % 26 + ord("A") + (i // 26) * ch_case)
        for j in range(resize[1]):
            psw[2] = chr(j % 26 + ord("A") + (j // 26) * ch_case)
            for k in range(resize[2]):
                psw[1] = chr(k % 26 + ord("A") + (k // 26) * ch_case)
                for l in range(resize[3]):
                    psw[0] = chr(l % 26 + ord("A") + (l // 26) * ch_case)
                    
                    if val == crypt.crypt("".join(psw[:n]), val[:2]):
                        return "".join(psw[:n])
            
if len(sys.argv) != 2:
    print("Usage: crack hash")
    exit(1)
    
size = 1
result = crack(sys.argv[1], size)
while not result and size != 4:
    size += 1
    result = crack(sys.argv[1], size)
print(result)



