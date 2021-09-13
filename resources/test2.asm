Data SEGMENT 
    FIRST DB 011111111b
    sec dw 0fffh
    Th1 DD 12
a db -128
b dw 1234h
c dw 0afffh
d dd -05
f dd 0ab123456h
Data ENDS

Code SEGMENT 
Jz begin
begin:
Jz begin
Stc
Dec eax
Dec bh
Pop dword ptr [edx + esi]
Pop dword ptr [ebp + esi]
Pop dword ptr ds:[ebp + esi]
Pop dword ptr ss:[ebp + esi]
Pop dword ptr gs:[ebp + esi]
Bsr eax,ebx
Bsr esp,ebx
jmp end_1
And al,[ebx+ecx]
Or dword ptr [ebx + esi] , ecx
Or byte ptr [ebp + esi] , ch
Or byte ptr ss:[ebp + esi] , ch
Or byte ptr ds:[ebp + esi] , ch
Or byte ptr es:[ebp + esi] , ch
Mov al,-1
Mov edx,-1
Add dword ptr cs:[edx + ecx],1fffffffh
Jz end_1
jmp begin
var dw 0fffh
jmp end_1
end_1:
jmp end_1
Jz begin
Or [ebp + esi] , ch
Code ENDS
END