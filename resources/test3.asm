Data SEGMENT 
    FIRST DB 1111111b
    sec dw 0fffh
    Th1 DD 12
a db 101010101b
b db -129
sec dd 1
c dw fffh
dddddd db -1
d dd 123456789h
Data ENDS

Code SEGMENT 

begin:
Stc Dec eax
Stc
Dec eax
Dec ax
Dec 
begin1:
Dec dword ptr [ebx + esi]
Pop dword ptr [edx + esi]
Pop  [edx + esi]
Bsr eax,ebx
Bsr al,bl
Bsr eax,ebx
Bsr eax,bl
jmp begin
jmp sec
And al,[esp+esp]
And al,[esp+ebp]
And al,[ebp+esp]
And al,[ebp+ebp]
And [ebp+ebp], al
And al,[esp+eax
Jz end_1
Or dword ptr [ebx + esi] , ecx
Mov al,0fh
Mov al,0fffh
Add dword ptr cs:[edx + ecx],1fffffffh
Add dword ptr al:[edx + ecx],1fffffffh
Jz end_1
jmp begin
var dw 0fffaah
Mov al,bl
end_1:
Code ENDS
END