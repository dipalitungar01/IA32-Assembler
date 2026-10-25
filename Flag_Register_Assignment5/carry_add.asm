; 0xFFFFFFFF + 1 wraps to zero and sets carry
MOV EAX, 0xFFFFFFFF
ADD EAX, 1
HLT
