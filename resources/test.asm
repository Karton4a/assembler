Data SEGMENT 
    FIRST DB 1111111b
    sec dw 0fffh
    Th1 DD 12
Data ENDS

Code SEGMENT 

begin:

Stc
Dec eax
Pop dword ptr [edx + esi]
Bsr eax,ebx
jmp end_1
And al,[ebx+ecx]
Or dword ptr [ebx + esi] , ecx
Mov al,0fh
Add dword ptr cs:[edx + ecx],1fffffffh
Jz end_1
jmp begin
var dw 0fffh
end_1:
Code ENDS
END